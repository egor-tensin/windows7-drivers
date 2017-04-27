Building drivers
================

The drivers can be built using [Windows Driver Kit 7.1.0].

[Windows Driver Kit 7.1.0]: https://www.microsoft.com/en-us/download/details.aspx?id=11800

Prerequisites
-------------

Install the "Build Environments" distributed with the kit, fire up `cmd`, and
execute (to target Windows 7/x86):

    C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1 fre WIN7 no_oacr

or (to target Windows 7/x86-64):

    C:\WinDDK\7600.16385.1\bin\setenv.bat C:\WinDDK\7600.16385.1 fre x64 WIN7 no_oacr

Manually
--------

Navigate to "km/src/" and execute:

    build.exe /cegwZ

This should build every driver, generating the .sys files somewhere in the
obj*something* directories in the respective drivers' directories.
They are not signed though.

Automatically
-------------

I developed a bunch of batch scripts to automatically build, sign and copy the
.sys files to a predefined directory.
To use them, execute `setenv.bat` (can be found in this directory).

### Code signing

The binaries are signed using a self-signed certificate issued by
"windows7_drivers".
This certificate must be stored in the current user's ROOT ("Trusted Root
Certification Authorities") store.
To generate such a certificate, execute `add_cert.bat`:

    add_cert.bat

To verify it's there, you can use the `certmgr.msc` utility.

The binaries are signed automatically after they are built, but you can also
sign manually by passing the path to a .sys file to `sign.bat`:

    sign.bat C:\workspace\personal\windows7-drivers\km\build\wdk7.1\bin\x64\Release\simple.sys

### Building the drivers

To build every driver, execute `build_drivers.bat`:

    build_drivers.bat

To build a particular driver, pass the path to the driver's source directory to
`build_driver.bat`:

    build_driver.bat C:\workspace\personal\windows7-drivers\km\src\simple

The binaries are copied to "bin/" (under this directory).

### Cleaning up

Cleaning up after building a driver includes deleting the log and object files
produced during the build as well as purging the binaries from the "bin/"
directory.

To clean up after `build_drivers.bat`, execute `clean_drivers.bat`:

    clean_drivers.bat

To clean up after building a particular driver, pass the driver's source
directory to `clean_driver.bat`:

    clean_driver.bat C:\workspace\personal\windows7-drivers\km\src\simple
