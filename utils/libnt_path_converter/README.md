# nt_path_converter driver utilities

[nt_path_converter] driver usage examples.

## Library

[nt_path_converter] driver's virtual device is wrapped into a separate library
`libnt_path_converter` using [libservice].
`#include <libnt_path_converter/all.hpp>` and link with
`libnt_path_converter.lib` to use the library.

## Utilities

### convert_nt_path.exe

    Usage: convert_nt_path.exe [NT_PATH...]

Converts a NT-style path to a DOS-style path.
The NT namespace can be explored using the [WinObj] utility.
For example:

    > convert_nt_path.exe \Device\HarddiskVolume2\Windows
    C:\Windows

## See also

* [Building]
* [License]



[building]: ../README.md#building
[license]: ../../README.md#license
[nt_path_converter]: ../../src/nt_path_converter
[libservice]: ../libservice/README.md
[WinObj]: https://technet.microsoft.com/en-us/library/bb896657.aspx
