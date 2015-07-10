@rem Copyright 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is licensed under the terms of the MIT License.
@rem See LICENSE.txt for details.

@setlocal enabledelayedexpansion
@echo off

set cert_name=windows_drivers

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
