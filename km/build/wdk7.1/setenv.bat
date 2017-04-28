@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@echo off

call check_ddk.bat || goto :ddk_not_set

set "proj_build_dir=%~dp0%"

set "proj_dir=%proj_build_dir%\..\..\.."
for /f "delims=" %%i in ("%proj_dir%") do set "proj_dir=%%~fi"

set "proj_src_dir=%proj_dir%\km\src"
set "proj_bin_dir=%proj_build_dir%\bin"
set "proj_lib_dir=%proj_build_dir%\lib"

if "%_BUILDARCH%" == "AMD64" (
    set target_arch=x64
)
if "%_BUILDARCH%" == "x86" (
    set target_arch=x86
)

if not defined target_arch goto :invalid_buildarch

if "%DDKBUILDENV%" == "fre" (
    set target_config=release
)
if "%DDKBUILDENV%" == "chk" (
    set target_config=debug
)

if not defined target_config goto :invalid_ddkbuildenv

exit /b 0

:ddk_not_set
exit /b %errorlevel%

:invalid_buildarch
echo Error: invalid %%_BUILDARCH%% value "%_BUILDARCH%" ^(the only supported architecture is x86^(-64^)^) >&2
exit /b 1

:invalid_ddkbuildenv
echo Error: invalid %%DDKBUILDENV%% value "%DDKBUILDENV%" >&2
exit /b 1
