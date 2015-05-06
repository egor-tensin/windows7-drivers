@setlocal enabledelayedexpansion
@echo off

for /f "delims=" %%i in ('dir /s sources /b') do (
  set driver_src_root=%%~dpi
  echo.
  call build_driver.bat "!driver_src_root:~0,-1!" || exit /b !errorlevel!
)
