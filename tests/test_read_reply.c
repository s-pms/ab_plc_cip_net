#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <limits.h>

#include "ab_cip.h"
#include "ab_cip_helper.h"
#include "socket.h"

static void peer_reply(int fd[2], byte* reply, int size)
{
	assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == 0);
	assert(send(fd[1], reply, size, 0) == size);
	assert(shutdown(fd[1], SHUT_WR) == 0);
}

static void make_reply(byte* reply, int size, int item_length)
{
	memset(reply, 0, size);
	reply[0] = 0x6f;
	reply[2] = (byte)(size - 24);
	reply[3] = (byte)((size - 24) >> 8);
	if (size >= 40) {
		reply[30] = 2;
		reply[36] = 0xb2;
		reply[38] = (byte)item_length;
		reply[39] = (byte)(item_length >> 8);
	}
	if (size >= 46) {
		reply[40] = 0xcc;
		reply[44] = CIP_Type_Word;
	}
}

static void forged_reply(void)
{
	byte reply[40];
	make_reply(reply, sizeof(reply), 0xffff);
	int fd[2];
	peer_reply(fd, reply, sizeof(reply));
	int length = -1;
	char* value = NULL;
	assert(ab_cip_read_string(fd[0], "Tag", &length, &value) != CIP_ERROR_CODE_SUCCESS);
	assert(length == 0 && value == NULL);
	close(fd[0]);
	close(fd[1]);
}

static void parser_boundaries(void)
{
	const int item_lengths[] = { 0, 1, 5, 6, 7, 8, 0xffff };
	for (int size = 0; size <= 48; size++) {
		for (unsigned int n = 0; n < sizeof(item_lengths) / sizeof(item_lengths[0]); n++) {
			byte* reply = (byte*)calloc(size ? size : 1, 1);
			assert(reply != NULL);
			if (size >= 24) make_reply(reply, size, item_lengths[n]);
			byte_array_info response = { 0 }, result = { 0 };
			response.data = reply;
			response.length = size;
			cip_error_code_e ret = cip_analysis_read_byte(response, &result);
			int payload = item_lengths[n] - 6;
			if (size >= 46 && payload > 0 && payload <= size - 46) {
				assert(ret == CIP_ERROR_CODE_SUCCESS && result.length == payload);
				assert(result.type == CIP_Type_Word);
				assert(memcmp(result.data, reply + 46, payload) == 0);
			}
			else {
				assert(ret != CIP_ERROR_CODE_SUCCESS);
				assert(result.data == NULL && result.length == 0);
			}
			free(result.data);
			free(reply);
		}
	}
}

static void valid_replies(void)
{
	byte reply[55];
	make_reply(reply, 48, 8);
	reply[46] = 0x34;
	reply[47] = 0x12;
	int fd[2];
	peer_reply(fd, reply, 48);
	short scalar = 0;
	assert(ab_cip_read_short(fd[0], "Tag", &scalar) == CIP_ERROR_CODE_SUCCESS);
	assert(scalar == 0x1234);
	close(fd[0]);
	close(fd[1]);

	make_reply(reply, sizeof(reply), 15);
	reply[44] = CIP_Type_Struct;
	reply[48] = 3; /* structure handle (2 bytes), then string length (4 bytes) */
	memcpy(reply + 52, "abc", 3);
	peer_reply(fd, reply, sizeof(reply));
	int length = 0;
	char* value = NULL;
	assert(ab_cip_read_string(fd[0], "Tag", &length, &value) == CIP_ERROR_CODE_SUCCESS);
	assert(length == 3 && strcmp(value, "abc") == 0);
	free(value);
	close(fd[0]);
	close(fd[1]);
}

static void invalid_replies(void)
{
	byte reply[48];
	byte_array_info response = { reply, sizeof(reply), 0 }, result = { 0 };
	const int offsets[] = { 0, 2, 8, 24, 30, 32, 34, 36, 40, 42, 43 };
	for (unsigned int i = 0; i < sizeof(offsets) / sizeof(offsets[0]); i++) {
		make_reply(reply, sizeof(reply), 8);
		reply[offsets[i]] ^= 1;
		assert(cip_analysis_read_byte(response, &result) != CIP_ERROR_CODE_SUCCESS);
		assert(result.data == NULL && result.length == 0);
	}
	make_reply(reply, sizeof(reply), 8);
	reply[40] = 0xcd;
	assert(cip_analysis_write_byte(response) == CIP_ERROR_CODE_SUCCESS);
	reply[42] = 5; /* A PLC rejection must never be reported as a successful write. */
	assert(cip_analysis_write_byte(response) != CIP_ERROR_CODE_SUCCESS);
	response.length = 1;
	assert(cip_analysis_write_byte(response) != CIP_ERROR_CODE_SUCCESS);
	response.data = NULL;
	response.length = 48;
	assert(cip_analysis_read_byte(response, &result) != CIP_ERROR_CODE_SUCCESS);
}

static void short_scalars(void)
{
	byte reply[53];
	int fd[2];
#define CHECK_SHORT(reader, type, bytes) do { \
	make_reply(reply, 46 + (bytes) - 1, 6 + (bytes) - 1); \
	peer_reply(fd, reply, 46 + (bytes) - 1); \
	type value = 17; \
	assert(reader(fd[0], "Tag", &value) != CIP_ERROR_CODE_SUCCESS); \
	assert(value == 17); \
	close(fd[0]); close(fd[1]); \
} while (0)
	CHECK_SHORT(ab_cip_read_short, short, 2);
	CHECK_SHORT(ab_cip_read_ushort, ushort, 2);
	CHECK_SHORT(ab_cip_read_int32, int32, 4);
	CHECK_SHORT(ab_cip_read_uint32, uint32, 4);
	CHECK_SHORT(ab_cip_read_int64, int64, 8);
	CHECK_SHORT(ab_cip_read_uint64, uint64, 8);
	CHECK_SHORT(ab_cip_read_float, float, 4);
	CHECK_SHORT(ab_cip_read_double, double, 8);
#undef CHECK_SHORT
	make_reply(reply, 47, 7);
	reply[44] = CIP_Type_Bool;
	reply[46] = 0xff;
	peer_reply(fd, reply, 47);
	bool value = false;
	assert(ab_cip_read_bool(fd[0], "Tag", &value) == CIP_ERROR_CODE_SUCCESS && value);
	close(fd[0]); close(fd[1]);
}

static void write_requests(void)
{
	byte reply[48], request[128];
	int fd[2];
	make_reply(reply, sizeof(reply), 4);
	reply[40] = 0xcd;
	peer_reply(fd, reply, sizeof(reply));
	assert(ab_cip_write_bool(fd[0], "Tag", false) == CIP_ERROR_CODE_SUCCESS);
	assert(socket_recv_data(fd[1], request, 68) == 68);
	assert(request[62] == 0 && request[63] == 0);
	close(fd[0]); close(fd[1]);

	reply[42] = 5;
	peer_reply(fd, reply, sizeof(reply));
	assert(ab_cip_write_string(fd[0], "Tag", 3, "abc") != CIP_ERROR_CODE_SUCCESS);
	close(fd[0]); close(fd[1]); /* LeakSanitizer covers the failed LEN write. */

	char long_tag[65537];
	memset(long_tag, 'A', sizeof(long_tag) - 1);
	long_tag[sizeof(long_tag) - 1] = 0;
	byte_array_info command = build_read_core_command(long_tag, 1);
	assert(command.data == NULL && command.length == 0);
	byte data = 0;
	byte_array_info input = { &data, 65535, 0 };
	command = build_write_core_command("Tag", CIP_Type_Byte, 1, input);
	assert(command.data == NULL && command.length == 0);
	input.length = 1;
	command = build_write_core_command("Tag", 0x1234, 1, input);
	assert(command.data != NULL && command.data[58] == 0x34 && command.data[59] == 0x12);
	free(command.data);
	assert(ab_cip_write_string(1, "Tag", 65535, "x") == CIP_ERROR_CODE_INVALID_PARAMETER);
}

static void* fragmented_peer(void* arg)
{
	int fd = *(int*)arg;
	byte reply[48];
	make_reply(reply, sizeof(reply), 8);
	reply[46] = 0x34;
	reply[47] = 0x12;
	assert(send(fd, reply, 1, 0) == 1);
	usleep(20000);
	assert(send(fd, reply + 1, 23, 0) == 23);
	usleep(20000);
	assert(send(fd, reply + 24, 24, 0) == 24);
	return NULL;
}

static void transport(void)
{
	int fd[2];
	assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == 0);
	pthread_t thread;
	assert(pthread_create(&thread, NULL, fragmented_peer, &fd[1]) == 0);
	short value = 0;
	assert(ab_cip_read_short(fd[0], "Tag", &value) == CIP_ERROR_CODE_SUCCESS);
	assert(value == 0x1234);
	assert(pthread_join(thread, NULL) == 0);
	close(fd[0]); close(fd[1]);

	/* Closing stdin may give a perfectly valid socket descriptor of zero. */
	int saved_stdin = dup(STDIN_FILENO);
	byte reply[48];
	make_reply(reply, sizeof(reply), 8);
	reply[46] = 42;
	peer_reply(fd, reply, sizeof(reply));
	assert(dup2(fd[0], STDIN_FILENO) == STDIN_FILENO);
	assert(ab_cip_read_short(STDIN_FILENO, "Tag", &value) == CIP_ERROR_CODE_SUCCESS && value == 42);
	if (saved_stdin >= 0) { assert(dup2(saved_stdin, STDIN_FILENO) == STDIN_FILENO); close(saved_stdin); }
	else close(STDIN_FILENO);
	close(fd[0]); close(fd[1]);

	assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == 0);
	close(fd[1]);
	signal(SIGPIPE, SIG_DFL);
	assert(socket_send_data(fd[0], "x", 1) == -1);
	close(fd[0]);
}

static void registration(void)
{
	byte reply[28] = { 0x65, 0, 4, 0, 1 };
	reply[24] = 1;
	int fd[2];
	byte_array_info request = { reply, sizeof(reply), 0 };
	uint32 session = 0;
	peer_reply(fd, reply, sizeof(reply));
	assert(read_data_from_server(fd[0], request, &session) && session == 1);
	close(fd[0]); close(fd[1]);
	reply[8] = 1;
	peer_reply(fd, reply, sizeof(reply));
	assert(!read_data_from_server(fd[0], request, &session) && session == 0);
	close(fd[0]); close(fd[1]);
	int output = 123;
	assert(!ab_cip_connect("127.0.0.1", 65536, 0, &output) && output == -1);
	assert(!ab_cip_connect("127.0.0.1", 44818, 256, &output) && output == -1);
}

static void signed_values(void)
{
	byte data[8];
	const int32 ints[] = { 0, -1, INT_MIN, INT_MAX };
	for (unsigned int i = 0; i < sizeof(ints) / sizeof(ints[0]); i++) {
		int2bytes(ints[i], data);
		assert(bytes2int32(data) == ints[i]);
	}
	const int64 longs[] = { 0, -1, LLONG_MIN, LLONG_MAX };
	for (unsigned int i = 0; i < sizeof(longs) / sizeof(longs[0]); i++) {
		bigInt2bytes(longs[i], data);
		assert(bytes2bigInt(data) == longs[i]);
	}
	assert(ntohf_(htonf_(-123.5f)) == -123.5f);
	assert(ntohd_(htond_(-123.5)) == -123.5);
	char text[66];
	const int invalid_radices[] = { INT_MIN, -37, -1, 0, 1, 37, INT_MAX };
	for (unsigned int i = 0; i < sizeof(invalid_radices) / sizeof(invalid_radices[0]); i++)
		assert(itoa(42, text, invalid_radices[i]) == NULL);
	assert(strcmp(itoa(ULLONG_MAX, text, 2), "1111111111111111111111111111111111111111111111111111111111111111") == 0);
	assert(strcmp(itoa((unsigned long long)LLONG_MIN, text, -10), "-9223372036854775808") == 0);
}

static void string_replies(void)
{
	byte reply[55];
	int fd[2];
	for (int n = 0; n < 3; n++) {
		make_reply(reply, sizeof(reply), 15);
		reply[44] = CIP_Type_Struct;
		uint2bytes(n == 0 ? 0 : (n == 1 ? 4 : 0xffffffffU), reply + 48);
		peer_reply(fd, reply, sizeof(reply));
		char* value = NULL;
		int length = -1;
		cip_error_code_e ret = ab_cip_read_string(fd[0], "Tag", &length, &value);
		if (n == 0) {
			assert(ret == CIP_ERROR_CODE_SUCCESS && length == 0 && strcmp(value, "") == 0);
			free(value);
		}
		else assert(ret != CIP_ERROR_CODE_SUCCESS && length == 0 && value == NULL);
		close(fd[0]); close(fd[1]);
	}
}

int main(int argc, char** argv)
{
	alarm(20);
	if (argc == 1 || strcmp(argv[1], "forged") == 0) forged_reply();
	if (argc == 1 || strcmp(argv[1], "boundaries") == 0) parser_boundaries();
	if (argc == 1 || strcmp(argv[1], "valid") == 0) valid_replies();
	if (argc == 1 || strcmp(argv[1], "invalid") == 0) invalid_replies();
	if (argc == 1 || strcmp(argv[1], "scalars") == 0) short_scalars();
	if (argc == 1 || strcmp(argv[1], "writes") == 0) write_requests();
	if (argc == 1 || strcmp(argv[1], "transport") == 0) transport();
	if (argc == 1 || strcmp(argv[1], "registration") == 0) registration();
	if (argc == 1 || strcmp(argv[1], "signed") == 0) signed_values();
	if (argc == 1 || strcmp(argv[1], "strings") == 0) string_replies();
	puts("CIP read reply tests passed");
	return 0;
}
