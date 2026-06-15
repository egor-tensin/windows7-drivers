param(
    [Parameter(Mandatory=$true)]
    [string] $InstallDir = $null,
    [Parameter(Mandatory=$true)]
    [string] $Platform = $null,
    [Parameter(Mandatory=$true)]
    [string] $Configuration = $null,
    [Parameter(Mandatory=$true)]
    [string] $WDK = $null
)

$ErrorActionPreference = "Stop";
Set-PSDebug -Strict

function Invoke-Exe {
    param(
        [ScriptBlock] $Cmd,
        [int[]] $AllowedExitCodes = @(0)
    )

    $backupErrorActionPreference = $script:ErrorActionPreference
    $script:ErrorActionPreference = 'Continue'

    try {
        & $Cmd
        if ($AllowedExitCodes -notcontains $LastExitCode) {
            throw "External command failed with exit code ${LastExitCode}: $Cmd"
        }
    } finally {
        $script:ErrorActionPreference = $backupErrorActionPreference
    }
}

function Test-AppVeyor {
    return Test-Path env:APPVEYOR
}

function Get-DriverBuildDir {
    param(
        [Parameter(Mandatory=$true)]
        [string] $DriverName
    )

    return "$script:PSScriptRoot\build\$script:WDK\$DriverName"
}

function Get-DriverSolutionPath {
    param(
        [Parameter(Mandatory=$true)]
        [string] $DriverName
    )

    $build_dir = Get-DriverBuildDir -DriverName $DriverName
    return "$build_dir\$DriverName.sln"
}

function Build-Driver {
    param(
        [Parameter(Mandatory=$true)]
        [string] $DriverName
    )

    $build_dir = Get-DriverBuildDir -DriverName $DriverName
    $solution_path = Get-DriverSolutionPath -DriverName $DriverName

    $msbuild_params = @()
    $msbuild_params += "/p:Platform=$script:Platform"
    $msbuild_params += "/p:Configuration=$script:Configuration"
    $msbuild_params += "/p:SignMode=TestSign"
    $msbuild_params += "/p:OutDir=$InstallDir\lib"

    cd $build_dir
    Invoke-Exe { msbuild.exe $msbuild_params $solution_path }
}

function Build-ProjectKernelMode {
    $drivers = 'minimal', 'simple', 'nt_namespace'

    foreach ($driver in $drivers) {
        Build-Driver -DriverName $driver
    }
}

function Build-Project {
    if (Test-AppVeyor) {
        $appveyor_cwd = pwd
    }

    New-Item -ItemType Directory -Force $script:InstallDir | Out-Null
    $script:InstallDir = (Resolve-Path $script:InstallDir).Path

    try {
        Build-ProjectKernelMode
    } finally {
        if (Test-AppVeyor) {
            cd $appveyor_cwd
            Set-PSDebug -Off
        }
    }
}

Build-Project
