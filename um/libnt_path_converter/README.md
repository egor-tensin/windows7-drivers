nt_path_converter driver utilities
==================================

[nt_path_converter] driver usage examples.

[nt_path_converter]: ../../km/src/special/nt_path_converter

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

* [Building the utilities]
* [License]

[Building the utilities]: ../README.md#building-the-utilities
[License]: ../../README.md#license
