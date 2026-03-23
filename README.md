[English README](README_EN.md)

## 版权与作者信息

- License: MIT（见 [LICENSE](LICENSE)）
- GitHub: iceman
- Email: wqliceman@gmail.com
- Copyright (c) 2022-2026 iceman

## 项目概述

- 项目名称：ab_plc_cip_net
- 开发语言：C
- 支持系统：Windows / Linux
- 测试设备：模拟 AB-CIP

本项目实现了基于 CIP（EtherNet/IP）协议的罗克韦尔 AB-PLC 通讯能力。
使用前请先在 PLC 侧正确配置以太网模块。

## 头文件

```c
#include "ab_cip.h"    // 协议接口
#include "typedef.h"   // 类型定义
```

## 连接参数

- port：端口号，通常为 44818
- plc_type：支持 1756 ControlLogix、1756 GuardLogix、1769 CompactLogix、5069 CompactLogix、Studio 5000 Logix Emulate 等型号

## 主要接口

### 1. 连接与断开

```c
byte get_plc_slot();
void set_plc_slot(byte slot);

bool ab_cip_connect(char* ip_addr, int port, int slot, int* fd);
bool ab_cip_disconnect(int fd);
```

### 2. 读取数据

```c
cip_error_code_e ab_cip_read_bool(int fd, const char* address, bool* val);
cip_error_code_e ab_cip_read_short(int fd, const char* address, short* val);
cip_error_code_e ab_cip_read_ushort(int fd, const char* address, ushort* val);
cip_error_code_e ab_cip_read_int32(int fd, const char* address, int32* val);
cip_error_code_e ab_cip_read_uint32(int fd, const char* address, uint32* val);
cip_error_code_e ab_cip_read_int64(int fd, const char* address, int64* val);
cip_error_code_e ab_cip_read_uint64(int fd, const char* address, uint64* val);
cip_error_code_e ab_cip_read_float(int fd, const char* address, float* val);
cip_error_code_e ab_cip_read_double(int fd, const char* address, double* val);
cip_error_code_e ab_cip_read_string(int fd, const char* address, int* length, char** val); // 需要释放 val
```

### 3. 写入数据

```c
cip_error_code_e ab_cip_write_bool(int fd, const char* address, bool val);
cip_error_code_e ab_cip_write_short(int fd, const char* address, short val);
cip_error_code_e ab_cip_write_ushort(int fd, const char* address, ushort val);
cip_error_code_e ab_cip_write_int32(int fd, const char* address, int32 val);
cip_error_code_e ab_cip_write_uint32(int fd, const char* address, uint32 val);
cip_error_code_e ab_cip_write_int64(int fd, const char* address, int64 val);
cip_error_code_e ab_cip_write_uint64(int fd, const char* address, uint64 val);
cip_error_code_e ab_cip_write_float(int fd, const char* address, float val);
cip_error_code_e ab_cip_write_double(int fd, const char* address, double val);
// 暂不支持（存在已知问题）
// cip_error_code_e ab_cip_write_string(int fd, const char* address, int length, const char* val);
```

## 使用示例

完整示例请参考 [ab_plc_cip_net/main.c](ab_plc_cip_net/main.c)。

最小连接示例：

```c
int fd = -1;
bool ok = ab_cip_connect("127.0.0.1", 44818, 0, &fd);
if (ok && fd > 0) {
    // do read/write
    ab_cip_disconnect(fd);
}
```
