Driver utilities
================

A couple of usage examples are included along with the drivers.

* [libservice]: Utilities to load/unload the drivers.
* [libsimple]: [simple] driver usage examples.
* [libnt_path_converter]: [nt_path_converter] driver usage examples.

[libservice]: libservice/README.md
[libsimple]: libsimple/README.md
[simple]: ../src/simple
[libnt_path_converter]: libnt_path_converter/README.md
[nt_path_converter]: ../src/nt_path_converter

Building the utilities
----------------------

Create the build files using CMake and build using Visual Studio.

For example, using Visual Studio 2013 Update 4 for Windows Desktop (targetting
x86):

    > cd
    C:\workspace\build\windows7-drivers

    > cmake -G "Visual Studio 12 2013" C:\workspace\personal\windows7-drivers\utils
    ...

    > msbuild windows7_drivers_utils.sln
    ...

See also
--------

* [License]

[License]: ../README.md#license
