#ifndef __H_TYPEDEF_H__
#define __H_TYPEDEF_H__

#include <stdint.h>
#include <stdbool.h>

typedef unsigned char byte;
typedef unsigned short ushort;
typedef signed int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef enum _tag_cip_error_code
{
	CIP_ERROR_CODE_OK = 0,				// 成功
	CIP_ERROR_CODE_FAILED = 1,			// 错误
	CIP_ERROR_CODE_MALLOC_FAILED = 2,	// 内存分配错误
	CIP_ERROR_CODE_UNKOWN = 99,			// 未知错误
} cip_error_code_e;

typedef enum _tag_cip_type
{
	CIP_Type_Bool = 0xC1,		// bool型数据，一个字节长度
	CIP_Type_Byte = 0xC2,		// byte型数据，一个字节长度，SINT
	CIP_Type_Word = 0xC3,		// 整型，两个字节长度，INT
	CIP_Type_DWord = 0xC4,		// 长整型，四个字节长度，DINT
	CIP_Type_LInt = 0xC5,		// 特长整型，8个字节，LINT
	CIP_Type_USInt = 0xC6,		// Unsigned 8-bit integer, USINT
	CIP_Type_UInt = 0xC7,		// Unsigned 16-bit integer, UINT
	CIP_Type_UDint = 0xC8,		// Unsigned 32-bit integer, UDINT
	CIP_Type_ULint = 0xC9,		// Unsigned 64-bit integer, ULINT
	CIP_Type_Real = 0xCA,		// 实数数据，四个字节长度
	CIP_Type_Double = 0xCB,		// 实数数据，八个字节的长度
	CIP_Type_Struct = 0xCC,		// 结构体数据，不定长度
	CIP_Type_String = 0xCD,		// 字符串数据内容
	CIP_Type_D1 = 0xD1,			// Bit string, 8 bits, BYTE
	CIP_Type_D2 = 0xD2,			// Bit string, 16-bits, WORD
	CIP_Type_D3 = 0xD3,			// Bit string, 32 bits, DWORD
	CIP_Type_D4 = 0xD4,			// Bit string, 64 bits LWORD
	CIP_Type_DA = 0xDA,			// 

}cip_type_e;

#endif // !__H_TYPEDEF_H__