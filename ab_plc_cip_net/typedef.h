/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2022-2026 iceman (wqliceman@gmail.com)
 * GitHub: iceman
 * This file is part of ab_plc_cip_net.
 */

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
	CIP_ERROR_CODE_SUCCESS = 0,				// Success
	CIP_ERROR_CODE_FAILED = 1,				// Error
	CIP_ERROR_CODE_MALLOC_FAILED = 2,		// Memory allocation failed
	CIP_ERROR_CODE_INVALID_PARAMETER = 3,	// Invalid parameter
	CIP_ERROR_CODE_UNKOWN = 99,				// Unknown error
} cip_error_code_e;

typedef enum _tag_cip_type
{
	CIP_Type_Bool = 0xC1,		// Boolean, 1 byte
	CIP_Type_Byte = 0xC2,		// Signed 8-bit integer (SINT), 1 byte
	CIP_Type_Word = 0xC3,		// Signed 16-bit integer (INT), 2 bytes
	CIP_Type_DWord = 0xC4,		// Signed 32-bit integer (DINT), 4 bytes
	CIP_Type_LInt = 0xC5,		// Signed 64-bit integer (LINT), 8 bytes
	CIP_Type_USInt = 0xC6,		// Unsigned 8-bit integer, USINT
	CIP_Type_UInt = 0xC7,		// Unsigned 16-bit integer, UINT
	CIP_Type_UDint = 0xC8,		// Unsigned 32-bit integer, UDINT
	CIP_Type_ULint = 0xC9,		// Unsigned 64-bit integer, ULINT
	CIP_Type_Real = 0xCA,		// Float, 4 bytes
	CIP_Type_Double = 0xCB,		// Double, 8 bytes
	CIP_Type_Struct = 0xCC,		// Struct, variable length
	CIP_Type_String = 0xCD,		// String content
	CIP_Type_D1 = 0xD1,			// Bit string, 8 bits, BYTE
	CIP_Type_D2 = 0xD2,			// Bit string, 16-bits, WORD
	CIP_Type_D3 = 0xD3,			// Bit string, 32 bits, DWORD
	CIP_Type_D4 = 0xD4,			// Bit string, 64 bits LWORD
	CIP_Type_DA = 0xDA,			// 

}cip_type_e;

#endif // !__H_TYPEDEF_H__