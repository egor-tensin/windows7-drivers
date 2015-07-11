# Windows drivers

This is a collection of drivers for Windows 7, created to amuse and entertain.

## Development

### Prerequisites

The drivers are compiled using the Windows Driver Kit Version 7.1.0.

I've developed a set of batch files to facilitate building the drivers and
cleaning after the build.
The binaries are copied into the "bin" directory under the root directory.

To set up the development environment, start a new `cmd` session, and run

    C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1 fre WIN7 no_oacr

Of course, you may want to modify `setenv.bat` parameters.
For example, to compile under x86-64, call `setenv.bat` like this:

    C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1 fre x64 WIN7 no_oacr

Then navigate to the root directory and call `setenv.bat`:

    setenv.bat

### Code signing

Driver binaries are signed using the self-signed certificate "Test Certificate"
in "Test Certificate Store".
You can create this certificate using `add_cert.bat` (but first make sure
you've set up the development environment).
To verify it's there, you can use the `certmgr.msc` utility.

Driver binaries are automatically signed during builds, but you can also sign
one manually by passing the path to a .sys file to `sign.bat`.

### Build & clean

To build every driver under the "src" directory, call `build_drivers.bat`.
To build a particular driver, pass the path to the driver source directory to
`build_driver.bat`.
Driver binaries are copied to the "bin" directory.

Cleaning after a driver build includes deleting log and object files in the
driver source directory and deleting the binaries from the "bin" directory.
To clean after every driver in the "src" directory, call `clean_drivers.bat`.
To clean after a particular driver, pass the path to the driver source
directory to `clean_driver.bat`.

## Installation

To install a driver as a service, you can use the `sc` utility.
For example, to install a driver "C:\test.sys" as a "test" service, run

    sc create test type= kernel binPath= C:\test.sys

You can then load/unload the driver using the `sc` to start/stop the
corresponding service.

    sc start test
    sc stop test

To uninstall a driver, delete the corresponding service using `sc`.

    sc delete test

Please note, that on the 64-bit version of Windows 7 loading 32-bit drivers is
not allowed.

You may also need to explicitly enable loading self-signed drivers on 64-bit
versions of Windows.
One way is to use the `bcdedit` utility:

    bcdedit /set testsigning on

Then restart your computer and you should be all set!

## Debugging

You can debug a driver using WinDbg.
To enable kernel debugging, you can use the `msconfig` utility (navigate to
"Boot" -> "Advanced options..." and check "Debug") or the `bcdedit` utility:

    bcdedit /debug on
    bcdedit /dbgsettings serial debugport:1 baudrate:115200

Restart your computer for these settings to take effect.

If a driver is loaded on a separate physical machine, you can connect to a
physical COM port from another host with WinDbg installed and enable kernel
debugging via "File" -> "Kernel Debug...".
You might need to restart the debuggee a couple of times in order to enter the
kernel debugging mode.

If a driver is running on a virtual machine, the conventional approach is to
expose a COM port via a named pipe.
You can then connect to the pipe from WinDbg installed on the host.
Refer to your virtualization software's documentation for more details.

## Licensing

This project, including all of the files and their contents, is licensed under
the terms of the MIT License.
See [LICENSE.txt](LICENSE.txt) for details.
