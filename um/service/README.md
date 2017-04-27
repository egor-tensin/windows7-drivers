Driver management utilities
===========================

Utilities to load/unload the drivers.

Usage
-----

### install_service.exe

    Usage: install_service.exe NAME SYS_PATH

Installs a driver as a service.
The same as

    > sc create NAME type= kernel binPath= SYS_PATH

### start_service.exe

    Usage: start_service.exe NAME

Starts the service `NAME` (loading the corresponding driver).
The same as

    > net start NAME

### stop_service.exe

    Usage: stop_service.exe NAME

Stops the service `NAME` (unloading the corresponding driver).
The same as

    > net stop NAME

### uninstall_service.exe

    Usage: uninstall_service.exe NAME

Uninstalls the service `NAME`, wiping the corresponding record from the
registry.
The same as

    > sc delete NAME

See also
--------

* [Building]
* [License]

[Building]: ../README.md#building
[License]: ../../README.md#license
