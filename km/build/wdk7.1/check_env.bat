@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@setlocal enabledelayedexpansion
@echo off

if not defined root goto :env_not_set
if not defined bin_root goto :env_not_set
if not defined lib_root goto :env_not_set
if not defined src_root goto :env_not_set

exit /b 0

:env_not_set
echo Error: either %%root%%, %%bin_root%%, %%lib_root%% or %%src_root%% are not set ^(have you set up the build environment using setenv.bat?^) >&2
exit /b 1
