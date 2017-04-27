@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

for /f "delims=" %%i in ('dir "%src_root%\sources" /b /s') do (
    set "driver_src_root=%%~dpi"
    echo.
    call build_driver.bat "!driver_src_root:~0,-1!" || exit /b !errorlevel!
)
