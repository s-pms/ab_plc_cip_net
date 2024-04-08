#ifdef _WIN32
#include <WinSock2.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

#define GET_RESULT(ret) { if (ret != 0) failed_count++;}

#include "ab_cip.h"

int main(int argc, char** argv)
{
#ifdef _WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}
#endif

	char* plc_ip = "127.0.0.1";
	int plc_port = 44818;
	if (argc > 1)
	{
		plc_ip = argv[1];
		plc_port = atoi(argv[2]);
	}

	int fd = -1;
	int slot = 0;
	bool ret = ab_cip_connect(plc_ip, plc_port, 0, &fd);
	if (ret && fd > 0)
	{
		cip_error_code_e ret = CIP_ERROR_CODE_FAILED;

		const int TEST_COUNT = 5000;
		const int TEST_SLEEP_TIME = 1000;
		int failed_count = 0;
		char address[50] = { 0 };
		int i = 0;

		for (i = 0; i < TEST_COUNT; i++)
		{
			printf("==============Test count: %d==============\n", i + 1);
			bool all_success = false;
			//////////////////////////////////////////////////////////////////////////
			bool val = true;
			strcpy(address, "E");
			ret = ab_cip_write_bool(fd, address, val);
			printf("Write\t %s \tbool:\t %d, \tret: %d\n", address, val, ret);
			GET_RESULT(ret);

			val = false;
			ret = ab_cip_read_bool(fd, address, &val);
			printf("Read\t %s \tbool:\t %d\n", address, val);
			GET_RESULT(ret);

			//////////////////////////////////////////////////////////////////////////
			short w_s_val = 23;
			strcpy(address, "A");
			ret = ab_cip_write_short(fd, address, w_s_val);
			printf("Write\t %s \tshort:\t %d, \tret: %d\n", address, w_s_val, ret);
			GET_RESULT(ret);

			short s_val = 0;
			ret = ab_cip_read_short(fd, address, &s_val);
			printf("Read\t %s \tshort:\t %d\n", address, s_val);
			GET_RESULT(ret);

			//////////////////////////////////////////////////////////////////////////
			ushort w_us_val = 255;
			strcpy(address, "A");
			ret = ab_cip_write_ushort(fd, address, w_us_val);
			printf("Write\t %s \tushort:\t %d, \tret: %d\n", address, w_us_val, ret);
			GET_RESULT(ret);

			ushort us_val = 0;
			ret = ab_cip_read_ushort(fd, address, &us_val);
			printf("Read\t %s \tushort:\t %d\n", address, us_val);
			GET_RESULT(ret);

			//////////////////////////////////////////////////////////////////////////
			int32 w_i_val = 12345;
			strcpy(address, "B");
			ret = ab_cip_write_int32(fd, address, w_i_val);
			printf("Write\t %s \tint32:\t %d, \tret: %d\n", address, w_i_val, ret);
			GET_RESULT(ret);

			int i_val = 0;
			ret = ab_cip_read_int32(fd, address, &i_val);
			printf("Read\t %s \tint32:\t %d\n", address, i_val);
			GET_RESULT(ret);

			//////////////////////////////////////////////////////////////////////////
			uint32 w_ui_val = 22345;
			ret = ab_cip_write_uint32(fd, address, w_ui_val);
			printf("Write\t %s \tuint32:\t %d, \tret: %d\n", address, w_ui_val, ret);
			GET_RESULT(ret);

			uint32 ui_val = 0;
			ret = ab_cip_read_uint32(fd, address, &ui_val);
			printf("Read\t %s \tuint32:\t %d\n", address, ui_val);
			GET_RESULT(ret);

			//////////////////////////////////////////////////////////////////////////
#if false
			int64 w_i64_val = 333334554;
			strcpy(address, "N");
			ret = ab_cip_write_int64(fd, address, w_i64_val);
			printf("Write\t %s \tuint64:\t %lld, \tret: %d\n", address, w_i64_val, ret);
			GET_RESULT(ret);

			int64 i64_val = 0;
			ret = ab_cip_read_int64(fd, address, &i64_val);
			printf("Read\t %s \tint64:\t %lld\n", address, i64_val);
			GET_RESULT(ret);

#endif
			//////////////////////////////////////////////////////////////////////////
			uint64 w_ui64_val = 4333334554;
			strcpy(address, "N");
			ret = ab_cip_write_uint64(fd, address, w_ui64_val);
			printf("Write\t %s \tuint64:\t %lld, \tret: %d\n", address, w_ui64_val, ret);
			GET_RESULT(ret);

			int64 ui64_val = 0;
			ret = ab_cip_read_uint64(fd, address, &ui64_val);
			printf("Read\t %s \tuint64:\t %lld\n", address, ui64_val);
			GET_RESULT(ret);

			//////////////////////////////////////////////////////////////////////////
			float w_f_val = 32.454f;
			strcpy(address, "C");
			ret = ab_cip_write_float(fd, address, w_f_val);
			printf("Write\t %s \tfloat:\t %f, \tret: %d\n", address, w_f_val, ret);
			GET_RESULT(ret);

			float f_val = 0;
			ret = ab_cip_read_float(fd, address, &f_val);
			printf("Read\t %s \tfloat:\t %f\n", address, f_val);
			GET_RESULT(ret);

			//////////////////////////////////////////////////////////////////////////
#if true
			double w_d_val = 12345.6789;
			ret = ab_cip_write_double(fd, address, w_d_val);
			printf("Write\t %s \tdouble:\t %lf, \tret: %d\n", address, w_d_val, ret);
			GET_RESULT(ret);

			double d_val = 0;
			ret = ab_cip_read_double(fd, address, &d_val);
			printf("Read\t %s \tdouble:\t %lf\n", address, d_val);
			GET_RESULT(ret);

#endif
			//////////////////////////////////////////////////////////////////////////
			const char sz_write[] = "wqliceman@gmail.com";
			int length = strlen(sz_write);
			strcpy(address, "F");
			ret = ab_cip_write_string(fd, address, length, sz_write);
			printf("Write\t %s \tstring:\t %s, \tret: %d\n", address, sz_write, ret);
			GET_RESULT(ret);

			char* str_val = NULL;
			ret = ab_cip_read_string(fd, address, &length, &str_val);
			printf("Read\t %s \tstring:\t %s\n", address, str_val);
			free(str_val);
			GET_RESULT(ret);

#ifdef _WIN32
			Sleep(TEST_SLEEP_TIME);
#else
			usleep(TEST_SLEEP_TIME * 1000);
#endif
		}

		printf("All Failed count: %d\n", failed_count);

		ab_cip_disconnect(fd);
		system("pause");
	}

#ifdef _WIN32
	WSACleanup();
#endif
}