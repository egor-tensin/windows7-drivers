nt_namespace driver utilities
==================================

[nt_namespace] driver usage examples.

[nt_namespace]: ../../../../km/src/special/nt_namespace

Usage
-----

### convert_nt_path.exe

    Usage: convert_nt_path.exe [NT_PATH...]

Converts a NT-style path to a DOS-style path.
The NT namespace can be explored using the [WinObj] utility.
For example:

    > convert_nt_path.exe \Device\HarddiskVolume2\Windows
    C:\Windows

[WinObj]: https://technet.microsoft.com/en-us/library/bb896657.aspx

See also
--------

* [Building]
* [License]

[Building]: ../../../README.md#building
[License]: ../../../../README.md#license
