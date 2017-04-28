@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

call check_env.bat || exit /b !errorlevel!

for /f "delims=" %%i in ('dir "%proj_src_dir%\sources" /b /s') do (
    set "drv_src_dir=%%~dpi"
    echo.
    call build_driver.bat "!drv_src_dir:~0,-1!" || exit /b !errorlevel!
)
