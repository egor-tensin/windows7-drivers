# libservice

Simple service management library and utilities.

## Usage

### libservice.lib

Clients may include the headers by `#include`ing `include/libservice/all.hpp`,
which includes all of the other header files.

For service management usage examples, see the utilities descriptions below.

For virtual device usage examples, refer e.g. to
[libnt_path_converter](../libnt_path_converter).

### install_service.exe

    Usage: install_service.exe NAME SYS_PATH

Installs a driver as a service.
The service is started manually.
The same as

    sc create NAME type= kernel binPath= SYS_PATH

### start_service.exe

    Usage: start_service.exe NAME

Starts a service (loading the corresponding driver).
The same as

    sc start NAME

except that `start_service.exe` waits until the service is actually loaded.

### stop_service.exe

    Usage: stop_service.exe NAME

Stops the service `NAME` (unloading the corresponding driver).
The same as

    sc stop NAME

except that `stop_service.exe` waits until the service is actually stopped.

### uninstall_service.exe

    Usage: uninstall_service.exe NAME

Uninstalls the service `NAME`, wiping the corresponding record from the
registry.
The same as

    sc delete NAME

## Building

See [Building](../README.md#building).

## Licensing

This project is licensed under the terms of the MIT License.
See [Licensing](../../README.md#licensing) for details.
