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
    Invoke-Exe { install_service.exe $DriverName "$InstallDir\lib\$DriverName.sys" }
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

    $actual = Exchange-Ints $NewInt
    if ($actual -ne $OldInt) {
        throw "Expected $OldInt, got: $actual"
    } else {
        Write-Host "Previous integer: $actual, new integer: $NewInt"
    }
}

function Convert-NTPath {
    param(
        [Parameter(Mandatory=$true)]
        [string] $NTPath
    )

    return Invoke-Exe { convert_nt_path.exe $NTPath }
}

function Verify-ConvertNTPath {
    param(
        [Parameter(Mandatory=$true)]
        [string] $NTPath,
        [Parameter(Mandatory=$true)]
        [string] $Expected
    )

    $actual = Convert-NTPath $NTPath
    if ($actual -ne $expected) {
        throw "Expected $expected, got: $actual"
    }
    Write-Host "NT path: $NTPath"
    Write-Host "DOS path: $actual"
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
    Verify-ConvertNTPath \DosDevices\C:\Windows C:\Windows

    foreach ($driver in $drivers) {
        Stop-Driver -DriverName $driver
    }
}

function Run-ProjectTestsAppVeyor {
    if (Test-AppVeyor) {
        $appveyor_cwd = pwd
    }

    $script:InstallDir = (Resolve-Path $script:InstallDir).Path

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
