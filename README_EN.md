## Project Overview

## Copyright and Author

- License: MIT (see [LICENSE](LICENSE))
- GitHub: iceman
- Email: wqliceman@gmail.com
- Copyright (c) 2022-2026 iceman

- Project Name: ab_plc_cip_net
- Development Language: C
- Supported Operating Systems: Windows, Linux
- Test Device: Simulated AB-CIP

The current implementation provides a Rockwell AB-PLC communication class utilizing the CIP (EtherNet/IP) protocol. Prior to usage, the Ethernet module on the PLC side must be configured accordingly.

#### Header Files
```c
#include "ab_cip.h"  // Provides protocol method interfaces
#include "typedef.h"   // Contains some type macro definitions
```

#### Connection Properties
- port: Port number, typically 44818
- plc_type: PLC model, compatible with models such as 1756 ControlLogix, 1756 GuardLogix, 1769 CompactLogix, 1769 Compact GuardLogix, 1789SoftLogix, 5069 CompactLogix, 5069 Compact GuardLogix, Studio 5000 Logix Emulate, etc.

#### PLC Address Classification
Supports tag-based read/write operations (current implementation is limited and does not fully encompass all protocol features).

## Implementation Details
#### 1. Connecting to PLC Devices

```c
byte get_plc_slot();          // Gets PLC slot number
void set_plc_slot(byte slot); // Sets the PLC slot number

bool ab_cip_connect(char* ip_addr, int port, int slot, int* fd); // Connects to a PLC device
bool ab_cip_disconnect(int fd); // Disconnects from the PLC
```

#### 2. Reading Data

```c
cip_error_code_e ab_cip_read_bool(int fd, const char* address, bool* val); // Reads a boolean value
cip_error_code_e ab_cip_read_short(int fd, const char* address, short* val); // Reads a short integer
cip_error_code_e ab_cip_read_ushort(int fd, const char* address, ushort* val); // Reads an unsigned short integer
cip_error_code_e ab_cip_read_int32(int fd, const char* address, int32* val); // Reads a 32-bit integer
cip_error_code_e ab_cip_read_uint32(int fd, const char* address, uint32* val); // Reads an unsigned 32-bit integer
cip_error_code_e ab_cip_read_int64(int fd, const char* address, int64* val); // Reads a 64-bit integer
cip_error_code_e ab_cip_read_uint64(int fd, const char* address, uint64* val); // Reads an unsigned 64-bit integer
cip_error_code_e ab_cip_read_float(int fd, const char* address, float* val); // Reads a floating-point value
cip_error_code_e ab_cip_read_double(int fd, const char* address, double* val); // Reads a double-precision floating-point value
cip_error_code_e ab_cip_read_string(int fd, const char* address, int* length, char** val); // Reads a string (requires freeing memory allocated for `val`)
```

### 3. Writing Data

```c
cip_error_code_e ab_cip_write_bool(int fd, const char* address, bool val); // Writes a boolean value
cip_error_code_e ab_cip_write_short(int fd, const char* address, short val); // Writes a short integer
cip_error_code_e ab_cip_write_ushort(int fd, const char* address, ushort val); // Writes an unsigned short integer
cip_error_code_e ab_cip_write_int32(int fd, const char* address, int32 val); // Writes a 32-bit integer
cip_error_code_e ab_cip_write_uint32(int fd, const char* address, uint32 val); // Writes an unsigned 32-bit integer
cip_error_code_e ab_cip_write_int64(int fd, const char* address, int64 val); // Writes a 64-bit integer
cip_error_code_e ab_cip_write_uint64(int fd, const char* address, uint64 val); // Writes an unsigned 64-bit integer
cip_error_code_e ab_cip_write_float(int fd, const char* address, float val); // Writes a floating-point value
cip_error_code_e ab_cip_write_double(int fd, const char* address, double val); // Writes a double-precision floating-point value
cip_error_code_e ab_cip_write_string(int fd, const char* address, int length, const char* val); // Writes a Logix STRING, length is the raw byte count
```

## Usage Example

## Build with Makefiles

This repository contains a two-level Makefile layout:

- Core library example source: [examples/ab_cip_test.c](examples/ab_cip_test.c)
- Build config [config.mk](config.mk): defines artifact locations, target names, and build mode flags.
- Shared rules [common.mk](common.mk): builds a static/shared library or executable from explicit source lists.
- Library makefile [ab_plc_cip_net/makefile](ab_plc_cip_net/makefile): builds the core `ab_plc_cip_net` target.
- Example makefile [examples/makefile](examples/makefile): links `ab_cip_test` against the generated library.

### Linux / WSL

From repository root:

```bash
make
make clean
```

The default `make` target builds the static library first and then links the example binary. Artifacts are written to:

- `artifacts/debug/lib/libab_plc_cip_net.a`
- `artifacts/debug/bin/ab_cip_test`

### Build options

Edit [config.mk](config.mk):

- `DEBUG=true` enables `-g` debug symbols.
- `DEBUG=false` builds in release mode.
- `BUILD_SHARED=true` switches the library makefile from static output to shared output.

Additional targets:

```bash
make lib
make shared
make example
make DEBUG=false
```

### Notes for Windows

- The Makefile flow assumes GNU Make (`make`) is available (for example through WSL or MSYS2).
- If you build with Visual Studio, use [ab_plc_cip_net/ab_plc_cip_net.sln](ab_plc_cip_net/ab_plc_cip_net.sln) instead. The solution now contains a library project and a separate example application project.

## Example Program

Refer to [examples/ab_cip_test.c](examples/ab_cip_test.c) for the complete sample application. The example is now intentionally separate from the core library target so downstream users can link the library without pulling in demo code.

Reading addresses follows the format "F" or "D".

```c
#include "ab_cip.h"

int fd = -1;
bool ok = ab_cip_connect("127.0.0.1", 44818, 0, &fd);
if (ok && fd > 0) {
    /* perform read and write operations here */
    ab_cip_disconnect(fd);
}
```
