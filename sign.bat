@setlocal enabledelayedexpansion
@echo off

set cert_store=Test Certificate Store
set cert_name=Test Certificate

if [%1] == [] (
  echo Usage: %0 SYS_PATH
  exit /b 1
)

set sys_path=%~f1

echo ============================ CERT INFO ============================
echo Certificate store: "%cert_store%"
echo Certificate name: "%cert_name%"
echo ========================== END CERT INFO ==========================
echo.
echo ============================= SIGNING =============================
echo signtool.exe sign /s "%cert_store%" /n "%cert_name%" "%sys_path%"
signtool.exe sign /s "%cert_store%" /n "%cert_name%" "%sys_path%" >nul || goto :signing_failure
echo signtool.exe verify /pa "%sys_path%"
signtool.exe verify /pa "%sys_path%" >nul || goto :signing_failure
echo ========================= SIGNING SUCCESS =========================
exit /b 0

:signing_failure
echo ========================= SIGNING FAILURE =========================
exit /b %errorlevel%
