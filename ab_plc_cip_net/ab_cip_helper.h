#ifndef __H_AB_CIP_HELPER_H__
#define __H_AB_CIP_HELPER_H__
#include "utill.h"

#define RELEASE_DATA(addr) \
    {                      \
        if (addr != NULL)  \
        {                  \
            free(addr);    \
        }                  \
    }
#define BUFFER_SIZE 1024
#define HEAD_SIZE 24

byte_array_info build_read_core_command(const char *address, int length);
byte_array_info build_write_core_command(const char *address, ushort typeCode, int length, byte_array_info value);

cip_error_code_e cip_analysis_read_byte(byte_array_info response, byte_array_info *ret);
cip_error_code_e cip_analysis_write_byte(byte_array_info response);

bool read_data_from_server(int fd, byte_array_info send, int *session);
bool cip_read_response(int fd, byte_array_info *response);

//////////////////////////////////////////////////////////////////////////
cip_error_code_e read_value(int fd, const char *address, int length, byte_array_info *out_bytes);
cip_error_code_e write_value(int fd, const char *address, int length, ushort type_code, byte_array_info in_bytes);

//////////////////////////////////////////////////////////////////////////

bool initialization_on_connect(int fd);

#endif //__H_AB_CIP_HELPER_H__