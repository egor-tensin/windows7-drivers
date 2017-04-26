@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

set cert_name=windows7_drivers

if [%1] == [] (
    echo Usage: %0 SYS_PATH
    exit /b 1
)

set sys_path=%~f1

echo ============================ CERT INFO ============================
echo Certificate name: "%cert_name%"
echo ========================== END CERT INFO ==========================
echo.
echo ============================= SIGNING =============================
call check_ddk.bat || goto :signing_failure
echo signtool.exe sign /s root /n "%cert_name%" "%sys_path%"
     signtool.exe sign /s root /n "%cert_name%" "%sys_path%" >nul || goto :signing_failure
echo signtool.exe verify /pa "%sys_path%"
     signtool.exe verify /pa "%sys_path%" >nul || goto :signing_failure
echo ========================= SIGNING SUCCESS =========================
exit /b 0

:signing_failure
echo ========================= SIGNING FAILURE =========================
exit /b %errorlevel%
