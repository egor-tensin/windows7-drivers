@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

set cert_name=windows7_drivers

echo ============================ CERT INFO ============================
echo Certificate name: "%cert_name%"
echo ========================== END CERT INFO ==========================
echo.
echo ======================= ADDING CERTIFICATE ========================
call check_ddk.bat || goto :add_cert_failure
echo makecert.exe -r -pe -ss root -n CN="%cert_name%" "%cert_name%.cer"
makecert.exe -r -pe -ss root -n CN="%cert_name%" "%cert_name%.cer" >nul || goto :add_cert_failure
echo del "%cert_name%.cer"
del "%cert_name%.cer" >nul || goto :add_cert_failure
echo =================== ADDING CERTIFICATE SUCCESS ====================
exit /b 0

:add_cert_failure
echo =================== ADDING CERTIFICATE FAILURE ====================
exit /b %errorlevel%
