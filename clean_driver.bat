@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

if "%~1" == "" (
    echo Usage: %~nx0 DRIVER_SRC_ROOT >&2
    exit /b 1
)

call check_ddk.bat || exit /b !errorlevel!
call check_env.bat || exit /b !errorlevel!

set "driver_src_root=%~f1"
cd "%driver_src_root%" || exit /b !errorlevel!

for /f %%i in ("%driver_src_root%") do (
    set "driver_name=%%~ni"
    set "driver_dist_root=%%~dpi"
)

call :make_relative driver_dist_root "%src_root%"

set "sys_dist_dir=%bin_root%\%target_platform%\%target_configuration%\%driver_dist_root%"
set "pdb_dist_dir=%bin_root%\%target_platform%\%target_configuration%\%driver_dist_root%"
set "lib_dist_dir=%lib_root%\%target_platform%\%target_configuration%\%driver_dist_root%"

echo =========================== DRIVER INFO ===========================
echo Driver source directory: %driver_src_root%
echo Driver name: %driver_name%
echo ========================= END DRIVER INFO =========================
echo.
echo ============================== CLEAN ==============================
call :clean_rmdir "obj%BUILD_ALT_DIR%"     || goto :clean_failure
call :clean_del "build%BUILD_ALT_DIR%.err" || goto :clean_failure
call :clean_del "build%BUILD_ALT_DIR%.log" || goto :clean_failure
call :clean_del "build%BUILD_ALT_DIR%.wrn" || goto :clean_failure
call :clean_del "%sys_dist_dir%%driver_name%.sys" || goto :clean_failure
call :clean_del "%pdb_dist_dir%%driver_name%.pdb" || goto :clean_failure
call :clean_del "%lib_dist_dir%%driver_name%.lib" || goto :clean_failure
echo ========================== CLEAN SUCCESS ==========================
exit /b 0

:clean_rmdir
if exist "%~1\" (
    echo rmdir /s /q "%~1"
         rmdir /s /q "%~1" >nul
    exit /b !errorlevel!
)
exit /b 0

:clean_del
if exist "%~1" (
    echo del "%~1"
         del "%~1" >nul
    exit /b !errorlevel!
)
exit /b 0

:clean_failure
echo ========================== CLEAN FAILURE ==========================
exit /b %errorlevel%

:make_relative
@setlocal enabledelayedexpansion

set "abs=%~1"
if defined %~1 set "abs=!%~1!"

set "base=%~2"
if not defined base set "base=%CD%"

for /f "tokens=*" %%i in ("%abs%") do set "abs=%%~fi"
for /f "tokens=*" %%i in ("%base%") do set "base=%%~fi"

set match=
set upper=

for /f "tokens=*" %%i in ('echo.%base:\=^&echo.%') do (
    set "sub=!sub!%%i\"
    call set "tmp=%%abs:!sub!=%%"
    if "!tmp!" == "!abs!" (
        set "upper=!upper!..\"
    ) else (
        set "match=!sub!"
    )
)

set "rel=%upper%!abs:%match%=!"

(endlocal & if defined %~1 (set "%~1=%rel%") else (echo.%rel%))

exit /b 0
