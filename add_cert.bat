@setlocal enabledelayedexpansion
@echo off

set cert_store=Test Certificate Store
set cert_name=Test Certificate

echo ============================ CERT INFO ============================
echo Certificate store: "%cert_store%"
echo Certificate name: "%cert_name%"
echo ========================== END CERT INFO ==========================
echo.
echo ======================= ADDING CERTIFICATE ========================
echo makecert.exe -$ individual -r -pe -ss "%cert_store%" -n CN="%cert_name%" "%cert_name%.cer"
makecert.exe -$ individual -r -pe -ss "%cert_store%" -n CN="%cert_name%" "%cert_name%.cer" >nul || goto :add_cert_failure
echo certmgr.exe /add "%cert_name%.cer" /s /r localMachine root
certmgr.exe /add "%cert_name%.cer" /s /r localMachine root >nul || goto :add_cert_failure
echo del "%cert_name%.cer"
del "%cert_name%.cer" >nul || goto :add_cert_failure
echo =================== ADDING CERTIFICATE SUCCESS ====================
exit /b 0

:add_cert_failure
echo =================== ADDING CERTIFICATE FAILURE ====================
exit /b %errorlevel%
