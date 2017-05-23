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

* **Customization.**
The runtime libraries are linked statically by default (when this project is
the root CMake project).
You can link the runtime dynamically by passing `-D USE_STATIC_RUNTIME=OFF` to
`cmake`.
* **Example.**
In the example below, the project directory is
"C:\workspace\personal\windows7-drivers\um" and Visual Studio 2013 is used,
targeting x86.

      > cmake -G "Visual Studio 12 2013" C:\workspace\personal\windows7-drivers\um
      ...

      > cmake --build . --config release -- /m
      ...
