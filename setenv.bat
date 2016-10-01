@rem Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
@rem This file is part of the "Windows 7 drivers" project.
@rem For details, see https://github.com/egor-tensin/windows7-drivers.
@rem Distributed under the MIT License.

@echo off

set root=%~dp0%
set src_root=%root%src
set bin_root=%root%bin
set lib_root=%root%lib

set target_configuration=Release

if [%_BUILDARCH%] == [AMD64] goto :target_platform_x64
if [%_BUILDARCH%] == [x86] goto :target_platform_x86
echo Unsupported %%_BUILDARCH%% (supported build architectures: AMD64, x86)
exit /b -1

:target_platform_x64
set target_platform=x64
exit /b 0
:target_platform_x86
set target_platform=x86
exit /b 0
