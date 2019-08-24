param(
    [string] $BuildDir = $null,
    [string] $ProjectDir = $null,
    [string] $Generator = $null,
    [string] $Platform = $null,
    [string] $Configuration = $null,
    [string] $DriverTargetOS = $null
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

function Set-AppVeyorDefaults {
    $script:ProjectDir = $env:APPVEYOR_BUILD_FOLDER
    $script:BuildDir = 'C:\Projects\build'
    $script:Generator = switch ($env:APPVEYOR_BUILD_WORKER_IMAGE) {
        'Visual Studio 2013' { 'Visual Studio 12 2013' }
        'Visual Studio 2015' { 'Visual Studio 14 2015' }
        'Visual Studio 2017' { 'Visual Studio 15 2017' }
        default { throw "Unsupported AppVeyor image: $env:APPVEYOR_BUILD_WORKER_IMAGE" }
    }
    $script:Platform = $env:PLATFORM
    $script:Configuration = $env:CONFIGURATION
    $script:DriverTargetOS = $env:appveyor_driver_target_os
}

function Build-ProjectUserMode {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $BuildDir,
        [Parameter(Mandatory=$true)]
        [string] $Generator,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration
    )

    mkdir $BuildDir
    cd $BuildDir

    Invoke-Exe { cmake.exe -Wno-dev -G $Generator -A $Platform "$ProjectDir\um" }
    Invoke-Exe { cmake.exe --build . --config $Configuration -- /m }
}

function Get-DriverConfiguration {
    param(
        [Parameter(Mandatory=$true)]
        [string] $Configuration,
        [Parameter(Mandatory=$true)]
        [string] $DriverTargetOS
    )

    return "$DriverTargetOS $Configuration"
}

function Get-DriverBuildDir {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $DriverSpec
    )

    return "$ProjectDir\km\build\wdk8.1update\$DriverSpec"
}

function Get-DriverSolutionPath {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $DriverSpec
    )

    $build_dir = Get-DriverBuildDir -ProjectDir $ProjectDir -DriverSpec $DriverSpec
    $driver_name = Split-Path -Path $build_dir -Leaf -Resolve
    return "$build_dir\$driver_name.vs12.sln"
}

function Build-Driver {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $DriverSpec,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration,
        [Parameter(Mandatory=$true)]
        [string] $DriverTargetOS
    )

    $build_dir = Get-DriverBuildDir -ProjectDir $ProjectDir -DriverSpec $DriverSpec
    $solution_path = Get-DriverSolutionPath -ProjectDir $ProjectDir -DriverSpec $DriverSpec

    $Configuration = Get-DriverConfiguration -Configuration $Configuration -DriverTargetOS $DriverTargetOS
    $msbuild_params = "/p:Platform=$Platform;Configuration=$Configuration;SignMode=TestSign"

    cd $build_dir
    Invoke-Exe { msbuild.exe $msbuild_params $solution_path }
}

function Build-ProjectKernelMode {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration,
        [Parameter(Mandatory=$true)]
        [string] $DriverTargetOS
    )

    $drivers = 'minimal', 'simple', 'special\nt_namespace'

    foreach ($driver in $drivers) {
        Build-Driver                      `
            -ProjectDir $ProjectDir       `
            -DriverSpec $driver           `
            -Platform $Platform           `
            -Configuration $Configuration `
            -DriverTargetOS $DriverTargetOS
    }
}

function Build-Project {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $BuildDir,
        [Parameter(Mandatory=$true)]
        [string] $Generator,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration,
        [Parameter(Mandatory=$true)]
        [string] $DriverTargetOS
    )

    Build-ProjectUserMode       `
        -ProjectDir $ProjectDir `
        -BuildDir $BuildDir     `
        -Generator $Generator   `
        -Platform $Platform     `
        -Configuration $Configuration
    Build-ProjectKernelMode           `
        -ProjectDir $ProjectDir       `
        -Platform $Platform           `
        -Configuration $Configuration `
        -DriverTargetOS $DriverTargetOS
}

function Build-ProjectAppVeyor {
    if (Test-AppVeyor) {
        Set-AppVeyorDefaults
        $appveyor_cwd = pwd
    }

    try {
        Build-Project                            `
            -ProjectDir $script:ProjectDir       `
            -BuildDir $script:BuildDir           `
            -Generator $script:Generator         `
            -Platform $script:Platform           `
            -Configuration $script:Configuration `
            -DriverTargetOS $script:DriverTargetOS
    } finally {
        if (Test-AppVeyor) {
            cd $appveyor_cwd
            Set-PSDebug -Off
        }
    }
}

Build-ProjectAppVeyor
