Windows 7 drivers
=================

[![AppVeyor branch](https://img.shields.io/appveyor/ci/egor-tensin/windows7-drivers/master?label=AppVeyor)](https://ci.appveyor.com/project/egor-tensin/windows7-drivers/branch/master)

A collection of basic Windows 7 drivers.

These don't actually manage any hardware, this is just an example of code
running in the kernel mode.

Development
-----------

* [Using WDK 7.1.0](km/build/wdk7.1/README.md)
* [Using WDK 8.1 Update](km/build/wdk8.1update/README.md)

Installation
------------

To install a driver as a Windows service, you can use the `sc` utility.
For example:

    sc create simple type= kernel binPath= C:\workspace\personal\windows7-drivers\km\build\wdk7.1\bin\x64\release\simple.sys

You can then load/unload the driver by starting/stopping the corresponding
service using the `net` utility.

    net start simple
    net stop simple

To uninstall a driver, delete the corresponding service using `sc`.

    sc delete simple

Please note that **64-bit versions of Windows 7 (or later) disallow loading
32-bit drivers**!

You may also need to explicitly enable self-signed drivers on 64-bit versions
of Windows.
Using the `bcdedit` utility, execute

    bcdedit /set testsigning on

and restart your computer.

Debugging
---------

A driver can be debugged using WinDbg.
To enable kernel debugging, you can use the `msconfig` utility (navigate to
"Boot" -> "Advanced options..." and check "Debug") or the `bcdedit` utility:

    bcdedit /debug on
    bcdedit /dbgsettings serial debugport:1 baudrate:115200

Then restart your computer for the new settings to take effect.

If a driver is loaded on a physical machine, you can connect to the COM port
specified in kernel debugging settings (#1 by default) with an actual cable and
enter the kernel debugging mode in WinDbg via "File" -> "Kernel Debug...".
You might need to restart the debuggee machine a couple of times in order to
enter the kernel debugging mode.

If a driver is loaded on a virtual machine, the conventional approach is to
expose one of the guest OS's COM ports via a named pipe.
You can then connect to the pipe from a WinDbg instance on the host OS (via
"File" -> "Kernel Debug...").
Refer to your virtualization software's documentation for details.

Utilities
---------

A couple of usages examples are included along with the drivers.
For details, see [Utilities].

[Utilities]: um/README.md

License
-------

Distributed under the MIT License.
See [LICENSE.txt] for details.

[LICENSE.txt]: LICENSE.txt
