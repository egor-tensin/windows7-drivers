Driver utilities
================

A couple of usage examples are included along with the drivers.

* [libservice]: Utilities to load/unload the drivers.
* [libsimple]: [simple] driver usage examples.
* [libnt_path_converter]: [nt_path_converter] driver usage examples.

[libservice]: service/README.md
[libsimple]: wrappers/simple/README.md
[simple]: ../km/src/simple
[libnt_path_converter]: wrappers/special/nt_path_converter/README.md
[nt_path_converter]: ../km/src/special/nt_path_converter

Building the utilities
----------------------

Create the build files using CMake and build using Visual Studio.

For example, using Visual Studio 2013 Update 4 for Windows Desktop (targetting
x86):

    > cd
    C:\workspace\build\windows7-drivers\um

    > cmake -G "Visual Studio 12 2013" C:\workspace\personal\windows7-drivers\um
    ...

    > cmake --build . --config release
    ...

See also
--------

* [License]

[License]: ../README.md#license
