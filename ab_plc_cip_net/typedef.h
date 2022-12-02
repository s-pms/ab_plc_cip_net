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
	CIP_ERROR_CODE_OK = 0,		// 成功
	CIP_ERROR_CODE_FAILED = 1,	// 错误
	CIP_ERROR_CODE_UNKOWN = 99, // 未知错误
} cip_error_code_e;

#endif // !__H_TYPEDEF_H__