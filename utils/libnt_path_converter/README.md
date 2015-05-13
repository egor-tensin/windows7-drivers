# libnt_path_converter

[nt_path_converter](https://github.com/egor-tensin/windows_drivers/tree/master/src/nt_path_converter) usage examples.

## Usage

### libnt_path_converter.lib

Wraps nt_path_converter's virtual device interface using [libservice](https://github.com/egor-tensin/windows_drivers/tree/master/utils/libservice).
Include the headers by `#include`ing `include/libnt_path_converter/all.hpp`, which includes all of the other header files.

### convert_nt_path.exe

    Usage: convert_nt_path.exe [NT_PATH...]

Converts a NT-style path to a DOS-style path.
The NT namespace can be explored using the [WinObj](https://technet.microsoft.com/en-us/library/bb896657.aspx) utility.
Usage example (assuming nt_path_converter is already loaded):

    >convert_nt_path.exe \Device\HarddiskVolume2\Windows
    C:\Windows

## Building

See [Building](https://github.com/egor-tensin/windows_drivers/tree/master/utils#building).

## Licensing

This project is licensed under the terms of the MIT License.
See [Licensing](https://github.com/egor-tensin/windows_drivers#licensing) for details.
