@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

if "%~1" == "" (
    echo Usage: %~nx0 DRIVER_SRC_DIR >&2
    exit /b 1
)

call check_env.bat || exit /b !errorlevel!

set "drv_src_dir=%~f1"

for /f %%i in ("%drv_src_dir%") do (
    set "drv_name=%%~ni"
    set "drv_subdir=%%~dpi"
)

call :make_relative drv_subdir "%proj_src_dir%"

set "sys_dist_dir=%proj_bin_dir%\%target_platform%\%target_configuration%\%drv_subdir%"
set "pdb_dist_dir=%proj_bin_dir%\%target_platform%\%target_configuration%\%drv_subdir%"
set "lib_dist_dir=%proj_lib_dir%\%target_platform%\%target_configuration%\%drv_subdir%"

echo =========================== DRIVER INFO ===========================
echo Driver source directory: %drv_src_dir%
echo Driver name: %drv_name%
echo ========================= END DRIVER INFO =========================
echo.
echo ============================== BUILD ==============================
set "LIBDISTDIR=%lib_dist_dir%"
pushd "%drv_src_dir%" && (
    build.exe /cegwZ
    popd
)
if errorlevel 0 (
    echo ========================== BUILD SUCCESS ==========================
) else (
    echo ========================== BUILD FAILURE ==========================
    exit /b %errorlevel%
)

if "%_BUILDARCH%" == "x86" (
    set buildarch_dir=i386
) else (
    set "buildarch_dir=%_BUILDARCH%"
)

set "sys_path=%drv_src_dir%\obj%BUILD_ALT_DIR%\%buildarch_dir%\%drv_name%.sys"
set "pdb_path=%drv_src_dir%\obj%BUILD_ALT_DIR%\%buildarch_dir%\%drv_name%.pdb"
set "lib_path=%drv_src_dir%\obj%BUILD_ALT_DIR%\%buildarch_dir%\%drv_name%.lib"

if exist "%sys_path%" (
    echo.
    call "%proj_build_dir%\sign.bat" "%sys_path%" || exit /b !errorlevel!
)

echo.
echo ============================== DISTR ==============================
if exist "%sys_path%" (
    call :distr_copy "%sys_path%" "%sys_dist_dir%" || goto :distr_failure
    call :distr_copy "%pdb_path%" "%pdb_dist_dir%" || goto :distr_failure
)
if exist "%lib_path%" (
    call :distr_copy "%lib_path%" "%lib_dist_dir%" || goto :distr_failure
)
echo ========================== DISTR SUCCESS ==========================
exit /b

:distr_mkdir
if not exist "%~1\" (
    echo mkdir "%~1"
         mkdir "%~1" >nul
    exit /b !errorlevel!
)
exit /b 0

:distr_copy
call :distr_mkdir "%~2" || exit /b !errorlevel!
echo copy "%~1" "%~2"
     copy "%~1" "%~2" >nul
exit /b %errorlevel%

:distr_failure
echo ========================== DISTR FAILURE ==========================
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
