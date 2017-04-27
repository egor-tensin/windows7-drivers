@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@echo off

set "root=%~dp0%"
set "src_root=%root%src"
set "bin_root=%root%bin"
set "lib_root=%root%lib"

set target_configuration=Release

if not defined _BUILDARCH (
    echo Error: %%_BUILDARCH%% is not defined ^(have you set up the WinDDK environment?^) >&2
    exit /b 1
)

if "%_BUILDARCH%" == "AMD64" (
    set target_platform=x64
    exit /b 0
)

if "%_BUILDARCH%" == "x86" (
    set target_platform=x86
    exit /b 0
)

echo Error: invalid %%_BUILDARCH%% value "%_BUILDARCH%" ^(the only supported architecture is x86^(-64^)^) >&2
exit /b 1
