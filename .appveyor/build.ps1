param(
    [string] $BuildDir = $null,
    [string] $ProjectDir = $null,
    [string] $Generator = $null,
    [string] $Platform = $null,
    [string] $Configuration = $null
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
        [string] $Configuration
    )

    mkdir $BuildDir
    cd $BuildDir

    Invoke-Exe { cmake.exe -Wno-dev -G $Generator -A $Platform "$ProjectDir\um" }
    Invoke-Exe { cmake.exe --build . --config $Configuration -- /m }
}

function Build-ProjectAppVeyor {
    if (Test-AppVeyor) {
        Set-AppVeyorDefaults
        $appveyor_cwd = pwd
    }

    try {
        Build-Project                      `
            -ProjectDir $script:ProjectDir `
            -BuildDir $script:BuildDir     `
            -Generator $script:Generator   `
            -Platform $script:Platform     `
            -Configuration $script:Configuration
    } finally {
        if (Test-AppVeyor) {
            cd $appveyor_cwd
        }
    }
}

Build-ProjectAppVeyor
