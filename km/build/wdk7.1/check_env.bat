@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

call check_ddk.bat || exit /b !errorlevel!

if not defined proj_build_dir goto :env_not_set
if not defined proj_bin_dir   goto :env_not_set
if not defined proj_lib_dir   goto :env_not_set
if not defined proj_src_dir   goto :env_not_set
if not defined target_config  goto :env_not_set
if not defined target_arch    goto :env_not_set

if not exist "%proj_build_dir%\sign.bat" (
    echo Error: %proj_build_dir%\sign.bat was not found ^(don^'t know how to sign drivers otherwise^) >&2
    exit /b 1
)

exit /b 0

:env_not_set
echo Error: either %%proj_build_dir%%, %%proj_bin_dir%%, %%proj_lib_dir%%, %%proj_src_dir%%, %%target_arch%% or %%target_config%% are not set ^(have set up the build environment using setenv.bat?^) >&2
exit /b 1
