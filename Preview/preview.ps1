# Builds and launches the standalone menu preview (no loader, no game, no DLL upload).
#
#   .\Preview\preview.ps1                 build Debug + run the game selected in Config.h
#   .\Preview\preview.ps1 -Game PUBG      preview another game without editing Config.h
#   .\Preview\preview.ps1 -Watch          rebuild + relaunch whenever a GUI/CheatSettings file is saved
#   .\Preview\preview.ps1 -All            compile every game's menu (no launch) and report pass/fail
#   .\Preview\preview.ps1 -Config Release
#
# Incremental: after the first build only the files you touched recompile (a few seconds).
param(
    [ValidateSet('', 'Apex', 'Arc', 'BO6', 'BO7Ext', 'BO7Int', 'Fortnite', 'Marvel', 'MW19', 'MW3', 'PUBG', 'R6', 'Rust', 'WarDogs')]
    [string]$Game = '',
    [ValidateSet('Debug', 'Release')] [string]$Config = 'Debug',
    [switch]$Watch,
    [switch]$NoRun,
    [switch]$All
)

$ErrorActionPreference = 'Stop'
$root     = Split-Path -Parent $PSScriptRoot
$project  = Join-Path $PSScriptRoot 'MenuPreview.vcxproj'
$allGames = @('Apex', 'Arc', 'BO6', 'BO7Ext', 'BO7Int', 'Fortnite', 'Marvel', 'MW19', 'MW3', 'PUBG', 'R6', 'Rust', 'WarDogs')

function Find-MSBuild {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $found = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
                            -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
        if ($found) { return $found }
    }
    $cmd = Get-Command msbuild -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    return $null
}

$msbuild = Find-MSBuild
if (-not $msbuild) {
    Write-Host 'No MSBuild with the C++ toolset found.' -ForegroundColor Red
    Write-Host 'Install VS Build Tools with the "Desktop development with C++" workload, e.g.:'
    Write-Host '  winget install Microsoft.VisualStudio.2022.BuildTools --override "--quiet --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"'
    exit 1
}

function Get-OutName([string]$g) { if ($g) { $g } else { 'Default' } }

function Invoke-Build([string]$g, [switch]$ErrorsOnly) {
    $props = @("/p:Configuration=$Config", '/p:Platform=x64', "/p:MenuGame=$(Get-OutName $g)")
    if ($g) {
        # Every Is<Game> macro is defined explicitly so Config.h's defaults don't apply.
        # '+'-separated because /p: splits on ';' and ','; the vcxproj turns '+' back into ';'.
        $defs = ($allGames | ForEach-Object { "Is$_=$([int]($_ -eq $g))" }) + 'IsValorant=0'
        $props += "/p:GameDefines=$($defs -join '+')"
    }
    # Out-Host: keep MSBuild's output on screen instead of in this function's return value
    if ($ErrorsOnly) {
        & $msbuild $project /nologo /m /v:minimal @props | Where-Object { $_ -match 'error ' } |
            ForEach-Object { $_ -replace ' \[[^\]]*\.vcxproj\]$', '' } | Select-Object -Unique | Out-Host
    } else {
        & $msbuild $project /nologo /m /v:minimal @props | Out-Host
    }
    return ($LASTEXITCODE -eq 0)
}

if ($All) {
    $failed = @()
    foreach ($g in $allGames) {
        Write-Host "=== $g ===" -ForegroundColor Cyan
        if (-not (Invoke-Build $g -ErrorsOnly)) { $failed += $g }
    }
    Write-Host ''
    if ($failed) { Write-Host "Failed: $($failed -join ', ')" -ForegroundColor Red; exit 1 }
    Write-Host 'All games compiled.' -ForegroundColor Green
    exit 0
}

$exe = Join-Path $PSScriptRoot "bin\$Config\$(Get-OutName $Game)\MenuPreview.exe"

function Start-Preview {
    if ($NoRun) { return $null }
    # Working dir = repo root so relative paths behave like they do under the loader
    return Start-Process -FilePath $exe -WorkingDirectory $root -PassThru
}

if (-not (Invoke-Build $Game)) { exit 1 }
$proc = Start-Preview
if (-not $Watch) { exit 0 }

$watched = @('MenuCustomization\GUI', 'MenuCustomization\CheatSettings') | ForEach-Object { Join-Path $root $_ }
function Get-Stamp {
    Get-ChildItem $watched -Recurse -Include *.cpp, *.h, *.hpp |
        Measure-Object -Property LastWriteTimeUtc -Maximum |
        ForEach-Object { $_.Maximum }
}

Write-Host "Watching GUI/ and CheatSettings/ for changes. Ctrl+C to stop." -ForegroundColor Cyan
$stamp = Get-Stamp
while ($true) {
    Start-Sleep -Milliseconds 700
    $now = Get-Stamp
    if ($now -eq $stamp) { continue }
    $stamp = $now
    Write-Host "`nChange detected, rebuilding..." -ForegroundColor Cyan
    if ($proc -and -not $proc.HasExited) { $proc.Kill(); $proc.WaitForExit() }
    if (Invoke-Build $Game) { $proc = Start-Preview }
    else { Write-Host 'Build failed; fix the error and save again.' -ForegroundColor Yellow }
}
