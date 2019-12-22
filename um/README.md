Driver utilities
================

A couple of usage examples are included along with the drivers.

* [libservice]: Utilities to load/unload the drivers.
* [libsimple]: [simple] driver usage examples.
* [libnt_namespace]: [nt_namespace] driver usage examples.

[libservice]: service/README.md
[libsimple]: wrappers/simple/README.md
[simple]: ../km/src/simple
[libnt_namespace]: wrappers/special/nt_namespace/README.md
[nt_namespace]: ../km/src/special/nt_namespace

Building
--------

Create the build files using CMake and build the project using Visual Studio.

The runtime libraries are linked statically by default.
You can link the runtime dynamically by passing `-D CC_STATIC_RUNTIME=OFF` to
`cmake`.
