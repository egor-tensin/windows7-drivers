# Service management utilities

Utilities used to manage (load, start, stop and unload) and communicate with drivers via virtual devices.
Usage examples of some of the drivers in this repository are also included.

* [libservice](https://github.com/egor-tensin/windows_drivers/tree/master/utils/libservice) &mdash; a simple service management library.
  Also includes means of communicating with a driver via virtual devices.
* [libnt_path_converter](https://github.com/egor-tensin/windows_drivers/tree/master/utils/libnt_path_converter) &mdash; [nt_path_converter](https://github.com/egor-tensin/windows_drivers/src/nt_path_converter) usage examples.

## Building

I've used the compiler shipped with Visual Studio Express 2013 with Update 4 for Windows Desktop.

You can generate the solution windows_drivers_utils.sln using CMake and build it using Visual Studio.

## Licensing

This project is licensed under the terms of the MIT License.
See [Licensing](https://github.com/egor-tensin/windows_drivers#licensing) for details.