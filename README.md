Windows 7 drivers
=================

A collection of basic Windows 7 drivers.

These are actually not drivers as such in the sense that they don't actually
manage any hardware.
The author prefers calling them *kernel modules*.

Development
-----------

### Prerequisites

The drivers are compiled using [Windows Driver Kit 7.1.0].
Install the "Build Environments" distributed with the kit, launch the Command
Prompt, and execute (to target x86):

    C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1 fre WIN7 no_oacr

or (to target x86-64):

    C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1 fre x64 WIN7 no_oacr

Then `cd` to the project's root directory and execute `setenv.bat`:

    setenv.bat

[Windows Driver Kit 7.1.0]: https://www.microsoft.com/en-us/download/details.aspx?id=11800

### Code signing

The binaries are signed using the self-signed certificate issued by
"windows7_drivers".
The certificate must be stored in the current user's ROOT ("Trusted Root
Certification Authorities") store.
To generate such a certificate, execute `add_cert.bat`:

    add_cert.bat

To verify it's there, you can use the `certmgr.msc` utility.

The binaries are signed automatically after they are built, but you can also
sign manually by passing the path to a .sys file to `sign.bat`:

    sign.bat C:\workspace\personal\windows7-drivers\bin\x64\Release\simple.sys

### Building the drivers

To build every driver under the "km/src/" directory, execute
`build_drivers.bat`:

    build_drivers.bat

To build a particular driver, pass the path to the driver's source directory to
`build_driver.bat`:

    build_driver.bat C:\workspace\personal\windows7-drivers\km\src\simple

Driver binaries are copied to the "bin/" directory under the project's root.

### Cleaning up

Cleaning up after building a driver includes deleting the log and object files
produced during the build as well as purging the binaries from the "bin/"
directory.

To clean up after building every driver in the "km/src/" directory, execute
`clean_drivers.bat`:

    clean_drivers.bat

To clean up after building a particular driver, pass the path to the driver's
source directory to `clean_driver.bat`:

    clean_driver.bat C:\workspace\personal\windows7-drivers\km\src\simple

Installation
------------

To install a driver as a Windows service, you can use the `sc` utility.
For example, to install `simple.sys` as a service with the name `simple`,
execute:

    sc create simple type= kernel binPath= C:\workspace\personal\windows7-drivers\bin\x64\Release\simple.sys

You can then load/unload the driver by starting/stopping the corresponding
service using the `net` utility.

    net start simple
    net stop simple

To uninstall a driver, delete the corresponding service using `sc`.

    sc delete simple

Please note, that **64-bit versions of Windows 7 disallow loading 32-bit
drivers**!

You may also need to explicitly enable loading self-signed drivers on 64-bit
versions of Windows.
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
specified in kernel debugging settings (#1 by default) with a proper cable
and enter the kernel debugging mode in WinDbg via "File" -> "Kernel Debug...".
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
