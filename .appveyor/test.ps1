param(
    [Parameter(Mandatory=$true)]
    [string] $InstallDir = $null
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

function Start-Driver {
    param(
        [Parameter(Mandatory=$true)]
        [string] $InstallDir,
        [Parameter(Mandatory=$true)]
        [string] $DriverName
    )

    Write-Host "Starting driver $DriverName..."
    Invoke-Exe { install_service.exe $DriverName "$InstallDir\lib\$DriverName.vs12.sys" }
    Invoke-Exe { start_service.exe $DriverName }
}

function Stop-Driver {
    param(
        [Parameter(Mandatory=$true)]
        [string] $DriverName
    )

    Write-Host "Stopping driver $DriverName..."
    Invoke-Exe { stop_service.exe $DriverName }
    Invoke-Exe { uninstall_service.exe $DriverName }
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
        [string] $InstallDir
    )

    $env:PATH = "$InstallDir\bin;${env:PATH}"

    $drivers = 'minimal', 'simple', 'nt_namespace'

    foreach ($driver in $drivers) {
        Start-Driver                `
            -InstallDir $InstallDir `
            -DriverName $driver
    }

    Verify-ExchangeInts -NewInt 69 -OldInt 42
    Verify-ExchangeInts -NewInt 100500 -OldInt 69

    foreach ($driver in $drivers) {
        Stop-Driver -DriverName $driver
    }
}

function Run-ProjectTestsAppVeyor {
    if (Test-AppVeyor) {
        $appveyor_cwd = pwd
    }

    try {
        Run-ProjectTests -InstallDir $script:InstallDir
    } finally {
        if (Test-AppVeyor) {
            cd $appveyor_cwd
            Set-PSDebug -Off
        }
    }
}

Run-ProjectTestsAppVeyor
