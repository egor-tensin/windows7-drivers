@setlocal enabledelayedexpansion
@echo off

if [%1] == [] (
  echo Usage: %~0 DRIVER_SRC_ROOT
  exit /b 1
)

call check_ddk.bat || exit /b !errorlevel!
call check_env.bat || exit /b !errorlevel!

set driver_src_root=%~f1
cd "%driver_src_root%"

for /f %%i in ("%driver_src_root%") do (
  set driver_name=%%~ni
  set driver_dist_root=%%~dpi
)

call :make_relative driver_dist_root "%src_root%"

set sys_dist_dir=%bin_root%\%target_platform%\%target_configuration%\%dist_root%
set pdb_dist_dir=%bin_root%\%target_platform%\%target_configuration%\%dist_root%
set lib_dist_dir=%lib_root%\%target_platform%\%target_configuration%\%dist_root%

echo =========================== DRIVER INFO ===========================
echo Driver source directory: "%driver_src_root%"
echo Driver name: "%driver_name%"
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
exit /b

:clean_rmdir
if exist "%~1\" (
  echo rmdir /s /q "%~1"...
  rmdir /s /q "%~1" >nul || exit /b !errorlevel!
)
exit /b

:clean_del
if exist "%~1" (
  echo del "%~1"
  del "%~1" >nul || exit /b !errorlevel!
)
exit /b

:clean_failure
echo ========================== CLEAN FAILURE ==========================
exit /b %errorlevel%

:make_relative
@setlocal enabledelayedexpansion
set src=%~1
if defined %1 set src=!%~1!
set base=%~2
if not defined base set base=%cd%
for /f "tokens=*" %%a in ("%src%") do set src=%%~fa
for /f "tokens=*" %%a in ("%base%") do set base=%%~fa
set match=
set c=
for /f "tokens=*" %%a in ('echo.%base:\=^&echo.%') do (
  set sub=!sub!%%a\
  call set tmp=%%src:!sub!=%%
  if "!tmp!" neq "!src!" (
    set match=!sub!
  ) else (
    set upper=!upper!..\
  )
)
set src=%upper%!src:%match%=!
(endlocal
if defined %1 (set %~1=%src%) else (echo.%src%))
exit /b
