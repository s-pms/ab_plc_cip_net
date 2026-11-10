#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ab_cip.h"
#include "ab_cip_helper.h"

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

int main(int argc, char** argv)
{
	if (argc == 1 || strcmp(argv[1], "forged") == 0) forged_reply();
	if (argc == 1 || strcmp(argv[1], "boundaries") == 0) parser_boundaries();
	if (argc == 1 || strcmp(argv[1], "valid") == 0) valid_replies();
	puts("CIP read reply tests passed");
	return 0;
}
