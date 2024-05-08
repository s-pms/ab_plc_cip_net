#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ab_cip_helper.h"
#include "socket.h"

extern uint32 g_session;
extern byte g_plc_slot;
extern byte g_registered_command[28];

// 从地址构造核心报文
byte_array_info build_read_core_command(const char* address, int length)
{
	size_t addr_length = strlen(address);
	size_t addr_adjust_length = addr_length;
	if (addr_adjust_length % 2 == 1)
		addr_adjust_length += 1;

	char* temp_address = (char*)malloc(addr_adjust_length);
	memset(temp_address, 0, addr_adjust_length);
	memcpy(temp_address, address, strlen(address));

	const ushort command_len = 9 + 26 + (ushort)addr_adjust_length + 1 + 24;
	byte* command = (byte*)malloc(command_len);
	memset(command, 0, command_len);

	command[0] = 0x6F; // 命令
	command[2] = (byte)((command_len - 24) % 256);
	command[3] = (byte)((command_len - 24) / 256); // 长度

	char temp_session[4] = { 0 };
	uint2bytes(g_session, temp_session);
	command[4] = temp_session[0];
	command[5] = temp_session[1];
	command[6] = temp_session[2];
	command[7] = temp_session[3]; // 会话句柄

	command[0 + 24] = 0x00;
	command[1 + 24] = 0x00;
	command[2 + 24] = 0x00;
	command[3 + 24] = 0x00;	// 接口句柄，默认为0x00000000（CIP）
	command[4 + 24] = 0x01;
	command[5 + 24] = 0x0A; // 超时（0x000A）
	command[6 + 24] = 0x02;
	command[7 + 24] = 0x00; // 项数（0x0002）
	command[8 + 24] = 0x00;
	command[9 + 24] = 0x00; // 空地址项（0x0000）
	command[10 + 24] = 0x00;
	command[11 + 24] = 0x00; // 长度（0x0000）
	command[12 + 24] = 0xB2; // type id   0xB2:UnConnected Data Item  0xB1:Connected Data Item  0xA1:Connect Address Item
	command[13 + 24] = 0x00; // 未连接数据项（0x00b2）
	command[14 + 24] = (byte)((command_len - 16 - 24) % 256); // 后面数据包的长度，等全部生成后在赋值
	command[15 + 24] = (byte)((command_len - 16 - 24) / 256);
	command[16 + 24] = 0x52; // 服务类型（0x03请求服务列表，0x52请求标签数据）
	command[17 + 24] = 0x02; // 请求路径大小
	command[18 + 24] = 0x20;
	command[19 + 24] = 0x06; // 请求路径(0x0620)
	command[20 + 24] = 0x24;
	command[21 + 24] = 0x01; // 请求路径(0x0124)
	command[22 + 24] = 0x0A;
	command[23 + 24] = 0xF0;
	command[24 + 24] = (byte)((6 + addr_adjust_length) % 256); // CIP指令长度
	command[25 + 24] = (byte)((6 + addr_adjust_length) / 256);

	command[0 + 24 + 26] = 0x4C; // 读取数据
	command[1 + 24 + 26] = (byte)((addr_adjust_length + 2) / 2);
	command[2 + 24 + 26] = 0x91;
	command[3 + 24 + 26] = (byte)addr_length;
	memcpy(command + 4 + 24 + 26, temp_address, addr_adjust_length);
	command[4 + 24 + 26 + addr_adjust_length] = (byte)((length) % 256);
	command[5 + 24 + 26 + addr_adjust_length] = (byte)((length) / 256);

	command[6 + 24 + 26 + addr_adjust_length] = 0x01;
	command[7 + 24 + 26 + addr_adjust_length] = 0x00;
	command[8 + 24 + 26 + addr_adjust_length] = 0x01;
	command[9 + 24 + 26 + addr_adjust_length] = g_plc_slot;

	byte_array_info ret = { 0 };
	ret.data = command;
	ret.length = command_len;
	return ret;
}

byte_array_info build_write_core_command(const char* address, ushort typeCode, int length, byte_array_info value)
{
	int val_len = 0;
	if (value.data != NULL)
		val_len = value.length;

	size_t addr_length = strlen(address);
	size_t addr_adjust_length = addr_length;
	if (addr_adjust_length % 2 == 1)
		addr_adjust_length += 1;

	char* temp_address = (char*)malloc(addr_adjust_length);
	memset(temp_address, 0, addr_adjust_length);
	memcpy(temp_address, address, strlen(address));

	const ushort command_len = 8 + 26 + (ushort)addr_adjust_length + val_len + 4 + 24;
	byte* command = (byte*)malloc(command_len);
	memset(command, 0, command_len);

	command[0] = 0x6F; // 命令
	command[2] = (byte)((command_len - 24) % 256);
	command[3] = (byte)((command_len - 24) / 256); // 长度

	char temp_session[4] = { 0 };
	uint2bytes(g_session, temp_session);
	command[4] = temp_session[0];
	command[5] = temp_session[1];
	command[6] = temp_session[2];
	command[7] = temp_session[3]; // 会话句柄

	command[0 + 24] = 0x00;
	command[1 + 24] = 0x00;
	command[2 + 24] = 0x00;
	command[3 + 24] = 0x00; // 接口句柄，默认为0x00000000（CIP）
	command[4 + 24] = 0x01;
	command[5 + 24] = 0x0A; // 超时（0x0001）
	command[6 + 24] = 0x02;
	command[7 + 24] = 0x00; // 项数（0x0002）
	command[8 + 24] = 0x00;
	command[9 + 24] = 0x00;
	command[10 + 24] = 0x00;
	command[11 + 24] = 0x00; // 空地址项（0x0000）
	command[12 + 24] = 0xB2;
	command[13 + 24] = 0x00; // 未连接数据项（0x00b2）
	command[14 + 24] = (byte)((command_len - 16 - 24) % 256); // 后面数据包的长度，等全部生成后在赋值
	command[15 + 24] = (byte)((command_len - 16 - 24) / 256);
	command[16 + 24] = 0x52; // 服务类型（0x03请求服务列表，0x52请求标签数据）
	command[17 + 24] = 0x02; // 请求路径大小
	command[18 + 24] = 0x20;
	command[19 + 24] = 0x06; // 请求路径(0x0620)
	command[20 + 24] = 0x24;
	command[21 + 24] = 0x01; // 请求路径(0x0124)
	command[22 + 24] = 0x0A;
	command[23 + 24] = 0xF0;
	command[24 + 24] = (byte)((8 + val_len + addr_adjust_length) % 256); // CIP指令长度
	command[25 + 24] = (byte)((8 + val_len + addr_adjust_length) / 256);

	command[0 + 26 + 24] = 0x4D; // 写数据
	command[1 + 26 + 24] = (byte)((addr_adjust_length + 2) / 2);
	command[2 + 26 + 24] = 0x91;
	command[3 + 26 + 24] = (byte)addr_length;
	memcpy(command + 4 + 26 + 24, temp_address, addr_adjust_length);
	command[4 + 26 + 24 + addr_adjust_length] = (byte)(typeCode % 256);
	command[5 + 26 + 24 + addr_adjust_length] = (byte)(typeCode) / 256;
	command[6 + 26 + 24 + addr_adjust_length] = (byte)(length % 256); // TODO length ??
	command[7 + 26 + 24 + addr_adjust_length] = (byte)(length / 256);
	memcpy(command + 8 + 26 + 24 + addr_adjust_length, value.data, value.length);

	command[8 + 26 + 24 + addr_adjust_length + val_len] = 0x01;
	command[9 + 26 + 24 + addr_adjust_length + val_len] = 0x00;
	command[10 + 26 + 24 + addr_adjust_length + val_len] = 0x01;
	command[11 + 26 + 24 + addr_adjust_length + val_len] = g_plc_slot;

	if (value.data != NULL)
		free(value.data);

	byte_array_info ret = { 0 };
	ret.data = command;
	ret.length = command_len;
	return ret;
}

cip_error_code_e cip_analysis_read_byte(byte_array_info response, byte_array_info* ret)
{
	cip_error_code_e ret_code = CIP_ERROR_CODE_SUCCESS;
	if (response.length == 0)
		return CIP_ERROR_CODE_FAILED;

	int temp_length = 0;
	int data_length = 0;
	if (response.length >= 40) // index 38 is data length[ushort]
	{
		data_length = bytes2ushort(response.data + 38);
		if (data_length > 6)
		{
			temp_length = data_length - 6;
			ret->data = (byte*)malloc(temp_length);
			memset(ret->data, 0, temp_length);
			memcpy(ret->data, response.data + 46, temp_length);
			ret->type = bytes2ushort(response.data + 44);
			ret->length = temp_length;
		}
	}
	else
	{
		ret_code = CIP_ERROR_CODE_UNKOWN;
	}
	return ret_code;
}

cip_error_code_e cip_analysis_write_byte(byte_array_info response)
{
	cip_error_code_e ret_code = CIP_ERROR_CODE_SUCCESS;
	if (response.length == 0)
		return CIP_ERROR_CODE_FAILED;

	return ret_code;
}

/// <summary>
/// 读取数据
/// 长度(length)默认为：1
/// </summary>
/// <param name="fd"></param>
/// <param name="address"></param>
/// <param name="length"></param>
/// <param name="out_bytes"></param>
/// <returns></returns>
cip_error_code_e read_value(int fd, const char* address, int length, byte_array_info* out_bytes)
{
	cip_error_code_e ret = CIP_ERROR_CODE_UNKOWN;
	byte_array_info core_cmd = build_read_core_command(address, length);
	if (core_cmd.data != NULL)
	{
		int need_send = core_cmd.length;
		int real_sends = socket_send_data(fd, core_cmd.data, need_send);
		if (real_sends == need_send)
		{
			byte temp[BUFFER_SIZE] = { 0 };
			memset(temp, 0, BUFFER_SIZE);
			byte_array_info response = { 0 };
			response.data = temp;
			response.length = BUFFER_SIZE;

			if (cip_read_response(fd, &response))
				ret = cip_analysis_read_byte(response, out_bytes);
		}
		free(core_cmd.data);
	}
	return ret;
}

cip_error_code_e write_value(int fd, const char* address, int length, ushort type_code, byte_array_info in_bytes)
{
	cip_error_code_e ret = CIP_ERROR_CODE_UNKOWN;
	byte_array_info core_cmd = build_write_core_command(address, type_code, length, in_bytes);
	if (core_cmd.data != NULL)
	{
		int need_send = core_cmd.length;
		int real_sends = socket_send_data(fd, core_cmd.data, need_send);
		if (real_sends == need_send)
		{
			byte temp[BUFFER_SIZE] = { 0 };
			memset(temp, 0, BUFFER_SIZE);
			byte_array_info response = { 0 };
			response.data = temp;
			response.length = BUFFER_SIZE;

			if (cip_read_response(fd, &response))
				ret = cip_analysis_write_byte(response);
		}
		free(core_cmd.data);
	}
	return ret;
}

bool initialization_on_connect(int fd)
{
	bool is_ok = false;
	g_session = 0;

	// First handshake -> send regiseter command
	byte_array_info temp = { 0 };
	int command_len = sizeof(g_registered_command);
	temp.data = (byte*)malloc(command_len);
	memcpy(temp.data, g_registered_command, command_len);
	temp.length = command_len;
	is_ok = read_data_from_server(fd, temp, &g_session);

	// Return a successful signal
	return is_ok;
}

bool cip_read_response(int fd, byte_array_info* response)
{
	bool is_ok = false;
	int nread = 0;
	int content_size = 0;

	if (fd < 0)
		return -1;

	byte* content = NULL;
	byte head[HEAD_SIZE];
	memset(head, 0, HEAD_SIZE);
	int recv_size = socket_recv_data_one_loop(fd, head, HEAD_SIZE);
	if (recv_size >= HEAD_SIZE) // header size
	{
		content_size = bytes2ushort(head + 2);
		if (content_size > 0)
		{
			content = (byte*)malloc(content_size);
			memset(content, 0, content_size);
		}
		recv_size = socket_recv_data(fd, content, content_size);
		if (recv_size == content_size)
		{
			response->length = HEAD_SIZE + content_size;
			response->data = (byte*)malloc(response->length);
			memset(response->data, 0, response->length);
			memcpy(response->data, head, HEAD_SIZE);
			memcpy(response->data + HEAD_SIZE, content, content_size);

			is_ok = true;
		}

		free(content);
	}
	return is_ok;
}

bool read_data_from_server(int fd, byte_array_info send, int* session)
{
	bool is_ok = false;
	int need_send = send.length;
	int real_sends = socket_send_data(fd, send.data, need_send);
	if (real_sends != need_send)
		return false;

	byte_array_info response = { 0 };
	if (cip_read_response(fd, &response))
	{
		if (response.length > 8)
		{
			*session = bytes2uint32(response.data + 4);
			is_ok = true;
		}

		RELEASE_DATA(response.data);
	}
	return is_ok;
}