#ifndef __H_AB_CIP_H__
#define __H_AB_CIP_H__

#include "typedef.h"

/////////////////////////////////////////////////////////////

byte get_plc_slot();
void set_plc_slot(byte slot);

/////////////////////////////////////////////////////////////

bool ab_cip_connect(char *ip_addr, int port, int slot, int *fd);
bool ab_cip_disconnect(int fd);

// read
cip_error_code_e ab_cip_read_bool(int fd, const char *address, bool *val);
cip_error_code_e ab_cip_read_short(int fd, const char *address, short *val);
cip_error_code_e ab_cip_read_ushort(int fd, const char *address, ushort *val);
cip_error_code_e ab_cip_read_int32(int fd, const char *address, int32 *val);
cip_error_code_e ab_cip_read_uint32(int fd, const char *address, uint32 *val);
cip_error_code_e ab_cip_read_int64(int fd, const char *address, int64 *val);
cip_error_code_e ab_cip_read_uint64(int fd, const char *address, uint64 *val);
cip_error_code_e ab_cip_read_float(int fd, const char *address, float *val);
cip_error_code_e ab_cip_read_double(int fd, const char *address, double *val);
cip_error_code_e ab_cip_read_string(int fd, const char *address, int *length, char **val); // need free val

// write
cip_error_code_e ab_cip_write_bool(int fd, const char *address, bool val);
cip_error_code_e ab_cip_write_short(int fd, const char *address, short val);
cip_error_code_e ab_cip_write_ushort(int fd, const char *address, ushort val);
cip_error_code_e ab_cip_write_int32(int fd, const char *address, int32 val);
cip_error_code_e ab_cip_write_uint32(int fd, const char *address, uint32 val);
cip_error_code_e ab_cip_write_int64(int fd, const char *address, int64 val);
cip_error_code_e ab_cip_write_uint64(int fd, const char *address, uint64 val);
cip_error_code_e ab_cip_write_float(int fd, const char *address, float val);
cip_error_code_e ab_cip_write_double(int fd, const char *address, double val);
cip_error_code_e ab_cip_write_string(int fd, const char *address, int length, const char *val);

#endif //__H_AB_CIP_H__