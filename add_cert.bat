@rem Copyright 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is licensed under the terms of the MIT License.
@rem See LICENSE.txt for details.

@setlocal enabledelayedexpansion
@echo off

set cert_name=win7drv

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
