param(
    [string] $BuildDir = $null,
    [string] $ProjectDir = $null,
    [string] $Configuration = $null,
    [string] $Platform = $null
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
        $result = & $Cmd
        if ($AllowedExitCodes -notcontains $LastExitCode) {
            throw "External command failed with exit code ${LastExitCode}: $Cmd"
        }
        return $result
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
    $script:Platform = $env:PLATFORM
    $script:Configuration = $env:CONFIGURATION
}

function Add-UtilsPath {
    param(
        [Parameter(Mandatory=$true)]
        [string] $BuildDir,
        [Parameter(Mandatory=$true)]
        [string] $Configuration
    )

    $env:PATH = "$BuildDir\service\utils\$Configuration;${env:PATH}"
    $env:PATH = "$BuildDir\wrappers\simple\utils\$Configuration;${env:PATH}"
}

function Get-DriverName {
    param(
        [Parameter(Mandatory=$true)]
        [string] $DriverSpec
    )

    return Split-Path -Path $DriverSpec -Leaf
}

function Get-DriverBinDir {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $DriverSpec,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration
    )

    $Platform = switch ($Platform) {
        'Win32' { 'x86' }
        'x64' { 'x64' }
        default { throw "Unsupported platform: $Platform" }
    }

    $driver_name = Get-DriverName -DriverSpec $DriverSpec

    return "$ProjectDir\km\build\wdk8.1update\$DriverSpec\bin\Windows7\$Platform\$Configuration"
}

function Get-DriverSysPath {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $DriverSpec,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration
    )

    $bin_dir = Get-DriverBinDir `
        -ProjectDir $ProjectDir `
        -DriverSpec $DriverSpec `
        -Platform $Platform     `
        -Configuration $Configuration
    $driver_name = Get-DriverName -DriverSpec $DriverSpec

    return "$bin_dir\$driver_name.vs12.sys"
}

function Start-Driver {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $DriverSpec,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration
    )

    $service_name = Get-DriverName -DriverSpec $DriverSpec
    $sys_path = Get-DriverSysPath `
        -ProjectDir $ProjectDir   `
        -DriverSpec $DriverSpec   `
        -Platform $Platform       `
        -Configuration $Configuration

    Write-Host "Starting driver $service_name..."
    Invoke-Exe { install_service.exe $service_name $sys_path }
    Invoke-Exe { start_service.exe $service_name }
}

function Stop-Driver {
    param(
        [Parameter(Mandatory=$true)]
        [string] $DriverSpec
    )

    $service_name = Get-DriverName -DriverSpec $DriverSpec

    Write-Host "Stopping driver $service_name..."
    Invoke-Exe { stop_service.exe $service_name }
    Invoke-Exe { uninstall_service.exe $service_name }
}

function Exchange-Ints {
    param(
        [Parameter(Mandatory=$true)]
        [int] $NewInt
    )

    return Invoke-Exe { exchange_ints.exe $NewInt }
}

function Verify-ExchangeInts {
    param(
        [Parameter(Mandatory=$true)]
        [int] $NewInt,
        [Parameter(Mandatory=$true)]
        [int] $OldInt
    )

    $old_int = Invoke-Exe { exchange_ints.exe $NewInt }
    if ($old_int -ne $OldInt) {
        throw "Expected $OldInt, got: $old_int"
    } else {
        Write-Host "Previous integer: $old_int, new integer: $NewInt"
    }
}

function Run-ProjectTests {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $BuildDir,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration
    )

    Add-UtilsPath -BuildDir $BuildDir -Configuration $Configuration
    Start-Driver                `
        -ProjectDir $ProjectDir `
        -DriverSpec 'simple'    `
        -Platform $Platform     `
        -Configuration $Configuration
    Verify-ExchangeInts -NewInt 69 -OldInt 42
    Verify-ExchangeInts -NewInt 100500 -OldInt 69
    Stop-Driver -DriverSpec 'simple'
}

function Run-ProjectTestsAppVeyor {
    if (Test-AppVeyor) {
        Set-AppVeyorDefaults
        $appveyor_cwd = pwd
    }

    try {
        Run-ProjectTests                   `
            -ProjectDir $script:ProjectDir `
            -BuildDir $script:BuildDir     `
            -Platform $script:Platform     `
            -Configuration $script:Configuration
    } finally {
        if (Test-AppVeyor) {
            cd $appveyor_cwd
            Set-PSDebug -Off
        }
    }
}

Run-ProjectTestsAppVeyor
