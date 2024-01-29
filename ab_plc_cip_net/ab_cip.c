#include "ab_cip_helper.h"
#include "ab_cip.h"
#include "ab_cip_private.h"

#include "socket.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib") /* Linking with winsock library */
#pragma warning(disable : 4996)
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

int port = 102;
char ip_address[64] = { 0 };

bool ab_cip_connect(char* ip_addr, int port, int slot, int* fd)
{
	bool ret = false;
	int temp_fd = -1;
	g_plc_slot = slot;

	temp_fd = socket_open_tcp_client_socket(ip_addr, port);
	*fd = temp_fd;

	if (temp_fd > 0)
		ret = initialization_on_connect(temp_fd);

	if (!ret && temp_fd > 0)
	{
		socket_close_tcp_socket(temp_fd);
		*fd = -1;
	}
	return ret;
}

bool ab_cip_disconnect(int fd)
{
	socket_close_tcp_socket(fd);
	return true;
}

//////////////////////////////////////////////////////////////////////////
cip_error_code_e ab_cip_read_bool(int fd, const char* address, bool* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 1, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length > 0)
	{
		*val = (bool)read_data.data[0];
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_short(int fd, const char* address, short* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 2, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length > 0)
	{
		*val = bytes2short(read_data.data);
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_ushort(int fd, const char* address, ushort* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 2, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length >= 2)
	{
		*val = bytes2ushort(read_data.data);
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_int32(int fd, const char* address, int32* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 4, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length >= 4)
	{
		*val = bytes2int32(read_data.data);
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_uint32(int fd, const char* address, uint32* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 4, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length >= 2)
	{
		*val = bytes2uint32(read_data.data);
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_int64(int fd, const char* address, int64* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 8, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length >= 8)
	{
		*val = bytes2bigInt(read_data.data);
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_uint64(int fd, const char* address, uint64* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 8, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length >= 8)
	{
		*val = bytes2ubigInt(read_data.data);
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_float(int fd, const char* address, float* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 4, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length >= 4)
	{
		*val = bytes2float(read_data.data);
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_double(int fd, const char* address, double* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	byte_array_info read_data;
	memset(&read_data, 0, sizeof(read_data));
	ret = read_value(fd, address, 8, &read_data);
	if (ret == CIP_ERROR_CODE_OK && read_data.length >= 8)
	{
		*val = bytes2double(read_data.data);
		RELEASE_DATA(read_data.data);
	}
	return ret;
}

cip_error_code_e ab_cip_read_string(int fd, const char* address, int* length, char** val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (length != NULL && *length > 0)
	{
		byte_array_info read_data;
		memset(&read_data, 0, sizeof(read_data));
		ret = read_value(fd, address, *length, &read_data);
		if (ret == CIP_ERROR_CODE_OK && read_data.length > 2)
		{
			*length = 0;
			uint32 str_length = read_data.data[0];
			if (str_length > 0)
			{
				*length = str_length;
				int temp_size = str_length + 1;
				char* ret_str = (char*)malloc(temp_size);
				if (ret_str != NULL)
				{
					memset(ret_str, 0, temp_size);
					memcpy(ret_str, read_data.data + 1, str_length);
					*val = ret_str;
				}
				else
				{
					ret = CIP_ERROR_CODE_MALLOC_FAILED;
				}
			}
			RELEASE_DATA(read_data.data);
		}
	}
	return ret;
}

cip_error_code_e ab_cip_write_bool(int fd, const char* address, bool val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 2;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		if (val)
		{
			write_data.data[0] = 0xFF;
			write_data.data[1] = 0xFF;
		}
		ret = write_value(fd, address, 1, 0xC1, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_short(int fd, const char* address, short val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 2;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		short2bytes(val, write_data.data);
		ret = write_value(fd, address, 1, 0xC3, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_ushort(int fd, const char* address, ushort val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 2;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		ushort2bytes(val, write_data.data);
		ret = write_value(fd, address, 1, 0xC3, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_int32(int fd, const char* address, int32 val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 4;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		int2bytes(val, write_data.data);
		ret = write_value(fd, address, 1, 0xC4, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_uint32(int fd, const char* address, uint32 val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 4;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		uint2bytes(val, write_data.data);
		ret = write_value(fd, address, 1, 0xC4, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_int64(int fd, const char* address, int64 val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 8;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		bigInt2bytes(val, write_data.data);
		ret = write_value(fd, address, 1, 0xC5, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_uint64(int fd, const char* address, uint64 val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 8;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		ubigInt2bytes(val, write_data.data);
		ret = write_value(fd, address, 1, 0xC5, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_float(int fd, const char* address, float val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 4;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		float2bytes(val, write_data.data);
		ret = write_value(fd, address, 1, 0xCA, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_double(int fd, const char* address, double val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL)
	{
		int write_len = 8;
		byte_array_info write_data;
		memset(&write_data, 0, sizeof(write_data));
		write_data.data = (byte*)malloc(write_len);
		write_data.length = write_len;

		double2bytes(val, write_data.data);
		ret = write_value(fd, address, 1, 0xCB, write_data);
	}
	return ret;
}

cip_error_code_e ab_cip_write_string(int fd, const char* address, int length, const char* val)
{
	cip_error_code_e ret = CIP_ERROR_CODE_FAILED;
	if (fd > 0 && address != NULL && val != NULL)
	{
		byte write_len = length + 1;
		byte_array_info write_data = { 0 };
		write_data.data = (byte*)malloc(write_len);
		if (write_data.data != NULL)
		{
			memset(write_data.data, 0, write_len);
			memcpy(write_data.data, &write_len, 1);
			memcpy(write_data.data + 1, val, length);
			write_data.length = write_len;

			char temp_addr[100] = { 0 };
			sprintf(temp_addr, "%s", address);
			ret = write_value(fd, temp_addr, 1, 0xD0, write_data);
		}
		else
		{
			ret = CIP_ERROR_CODE_MALLOC_FAILED;
		}
	}
	return ret;
}

byte get_plc_slot()
{
	return g_plc_slot;
}

void set_plc_slot(byte slot)
{
	g_plc_slot = slot;
}