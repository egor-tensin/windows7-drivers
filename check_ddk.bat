@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

call check_env.bat || exit /b !errorlevel!

if not defined BUILD_ALT_DIR goto :ddk_not_set
if not defined _BUILDARCH goto :ddk_not_set

if not exist "%root%\sign.bat" (
    echo Error: %root%\sign.bat was not found ^(don^'t know how to sign drivers otherwise^) >&2
    exit /b 1
)

where build.exe >nul 2>&1 || goto :build_not_found
where signtool.exe >nul 2>&1 || goto :signtool_not_found
where makecert.exe >nul 2>&1 || goto :makecert_not_found
exit /b 0

:ddk_not_set
echo Error: either %%BUILD_ALT_DIR%% or %%_BUILDARCH%% are not set ^(have you set up the WinDDK environment?^) >&2
exit /b 1

:build_not_found
echo Error: build.exe was not found ^(have you set up the WinDDK environment?^) >&2
exit /b 1

:signtool_not_found
echo Error: signtool.exe was not found ^(have you set up the WinDDK environment?^) >&2
exit /b 1

:makecert_not_found
echo Error: makecert.exe was not found ^(have you set up the WinDDK environment?^) >&2
exit /b 1
