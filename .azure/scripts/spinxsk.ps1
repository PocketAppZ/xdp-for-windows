<#

.SYNOPSIS
This script runs spinxsk.exe.

.PARAMETER Config
    Specifies the build configuration to use.

.PARAMETER Arch
    The CPU architecture to use.

.PARAMETER Minutes
    Duration of execution in minutes.

.PARAMETER Stats
    Periodic socket statistics output.

.PARAMETER QueueCount
    Number of queues to spin.

.Parameter FuzzerCount
    Number of fuzzer threads per queue.

.PARAMETER CleanDatapath
    Avoid actions that invalidate the datapath.

#>

param (
    [Parameter(Mandatory = $false)]
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug",

    [Parameter(Mandatory = $false)]
    [ValidateSet("x86", "x64", "arm", "arm64")]
    [string]$Arch = "x64",

    [Parameter(Mandatory = $false)]
    [Int32]$Minutes = 0,

    [Parameter(Mandatory = $false)]
    [switch]$Stats = $false,

    [Parameter(Mandatory = $false)]
    [Int32]$QueueCount = 0,

    [Parameter(Mandatory = $false)]
    [Int32]$FuzzerCount = 0,

    [Parameter(Mandatory = $false)]
    [switch]$CleanDatapath = $false
)

Set-StrictMode -Version 'Latest'
$PSDefaultParameterValues['*:ErrorAction'] = 'Stop'

# Important paths.
$RootDir = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$ArtifactsDir = Join-Path $RootDir "artifacts" "bin" "$($Arch)_$($Config)"
$SpinXsk = Join-Path $ArtifactsDir "spinxsk.exe"

Write-Host "+++++++ Running SpinXsk.exe +++++++"

# Verify all the files are present.
if (!(Test-Path $SpinXsk)) {
    Write-Error "$SpinXsk does not exist!"
}

# Build up the args.
$Args = "-IfIndex $((Get-NetAdapter XDPMP).ifIndex)"
if ($Minutes -ne 0) {
    $Args += " -Minutes $Minutes"
}
if ($Stats) {
    $Args += " -Stats"
}
if ($QueueCount -ne 0) {
    $Args += " -QueueCount $QueueCount"
}
if ($FuzzerCount -ne 0) {
    $Args += " -FuzzerCount $FuzzerCount"
}
if ($CleanDatapath) {
    $Args += " -CleanDatapath"
}

# Run the exe.
Write-Debug ($SpinXsk + " " + $Args)
Invoke-Expression ($SpinXsk + " " + $Args)