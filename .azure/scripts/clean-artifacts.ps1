<#

.SYNOPSIS
This cleans up duplicate build artifacts for CI upload. Note that user mode
artifacts are left in the base directory.

.PARAMETER Config
    Specifies the build configuration to use.

.PARAMETER Arch
    The CPU architecture to use.

#>

param (
    [Parameter(Mandatory = $false)]
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug",

    [Parameter(Mandatory = $false)]
    [ValidateSet("x86", "x64", "arm", "arm64")]
    [string]$Arch = "x64"
)

Set-StrictMode -Version 'Latest'
$PSDefaultParameterValues['*:ErrorAction'] = 'Stop'

# Artifact paths.
$RootDir = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$ArtifactsDir = Join-Path $RootDir "artifacts" "bin" "$($Arch)_$($Config)"

# Move files into the specific driver folders.
Move-Item (Join-Path $ArtifactsDir "fndis.lib") (Join-Path $ArtifactsDir "fndis" "fndis.lib")

# Delete all unnecessary duplicates.
Remove-Item (Join-Path $ArtifactsDir "fndis.sys")
Remove-Item (Join-Path $ArtifactsDir "xdp.inf")
Remove-Item (Join-Path $ArtifactsDir "xdp.sys")
Remove-Item (Join-Path $ArtifactsDir "xdpfnmp.inf")
Remove-Item (Join-Path $ArtifactsDir "xdpfnmp.sys")
Remove-Item (Join-Path $ArtifactsDir "xdpmp.inf")
Remove-Item (Join-Path $ArtifactsDir "xdpmp.sys")
Remove-Item (Join-Path $ArtifactsDir "xdpmp" "fndis.sys")