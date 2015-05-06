@setlocal enabledelayedexpansion
@echo off

if [%root%] == [] goto :env_not_set
if [%bin_root%] == [] goto :env_not_set
if [%lib_root%] == [] goto :env_not_set
if [%src_root%] == [] goto :env_not_set

exit /b 0

:env_not_set
echo Error: either %%root%%, %%bin_root%%, %%lib_root%% or %%src_root%% are not set ^(perhaps you forgot to call setenv.bat^)
exit /b 1
