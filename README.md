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
cip_error_code_e ab_cip_write_string(int fd, const char* address, int length, const char* val); // length 为字符串原始字节数
```

## 使用示例

完整示例请参考 [examples/ab_cip_test.c](examples/ab_cip_test.c)。

最小连接示例：

```c
int fd = -1;
bool ok = ab_cip_connect("127.0.0.1", 44818, 0, &fd);
if (ok && fd > 0) {
    // do read/write
    ab_cip_disconnect(fd);
}
```

## 构建方式

当前仓库已拆分为两个公开目标：

- 核心库：`ab_plc_cip_net`
- 示例程序：`ab_cip_test`

### 使用 Make 构建

默认执行：

```bash
make
```

会先生成静态库，再链接示例程序，产物位于：

- `artifacts/debug/lib/libab_plc_cip_net.a`
- `artifacts/debug/bin/ab_cip_test`

常用命令：

```bash
make lib                 # 仅构建静态库
make shared              # 构建共享库 libab_plc_cip_net.so
make example             # 构建并链接示例程序
make clean               # 清理所有 artifacts
make DEBUG=false         # 以 release 模式构建默认目标
```

### 使用 Visual Studio 构建

解决方案 [ab_plc_cip_net/ab_plc_cip_net.sln](ab_plc_cip_net/ab_plc_cip_net.sln) 现在包含两个工程：

- `ab_plc_cip_net`：生成静态库
- `ab_cip_test`：链接静态库并生成示例程序

MSVC 产物默认输出到 `artifacts/msvc/<Platform>/<Configuration>/` 目录下。
