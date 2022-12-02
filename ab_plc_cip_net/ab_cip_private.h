#ifndef __H_AB_CIP_PRIVATE_H__
#define __H_AB_CIP_PRIVATE_H__
#include "typedef.h"

/// <summary>
/// 注册命令
/// </summary>
const byte g_registered_command[] =
	{
		0x65, 0x00,										// 注册请求
		0x04, 0x00,										// 命令数据长度(单位字节)
		0x00, 0x00, 0x00, 0x00,							// 会话句柄,初始值为0x00000000
		0x00, 0x00, 0x00, 0x00,							// 状态，初始值为0x00000000（状态好）
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 请求通信一方的说明
		0x00, 0x00, 0x00, 0x00,							// 选项，默认为0x00000000
		0x01, 0x00,										// 协议版本（0x0001）
		0x00, 0x00										// 选项标记（0x0000
};

///////////////////////////////////////////////////////////////////////////

// 会话句柄(由AB PLC生成)
uint32 g_session;

byte g_plc_slot;

#endif //__H_AB_CIP_PRIVATE_H__
