[CmdletBinding()]
param(
    [switch]$WhatIf
)

$ErrorActionPreference = 'Stop'

# This script is stored in <repository>\scripts, so its parent is always the Git root.
$RepositoryPath = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$LogDirectory = Join-Path $RepositoryPath 'backup-logs'
$Timestamp = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
$CommitDate = Get-Date -Format 'yyyy-MM-dd'

New-Item -ItemType Directory -Path $LogDirectory -Force | Out-Null
$LogPath = Join-Path $LogDirectory ("backup-{0}.log" -f (Get-Date -Format 'yyyy-MM-dd'))

function Write-BackupLog {
    param([string]$Message)
    $line = "[{0}] {1}" -f (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'), $Message
    $line | Tee-Object -FilePath $LogPath -Append
}

function Invoke-Git {
    param([Parameter(ValueFromRemainingArguments = $true)][string[]]$Arguments)
    & git -C $RepositoryPath @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "git $($Arguments -join ' ') failed with exit code $LASTEXITCODE."
    }
}

try {
    if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
        throw 'Git was not found in PATH.'
    }

    Invoke-Git config --get remote.origin.url | Out-Null
    $Branch = (& git -C $RepositoryPath branch --show-current).Trim()
    if ([string]::IsNullOrWhiteSpace($Branch)) {
        throw 'The repository is in detached HEAD state; automatic backup was not run.'
    }

    # Avoid unexpectedly mixing this backup with work a user has explicitly staged.
    $ExistingStagedChanges = & git -C $RepositoryPath diff --cached --quiet
    if ($LASTEXITCODE -eq 1) {
        Write-BackupLog 'Skipped: existing staged changes detected. Commit or unstage them, then run again.'
        exit 2
    }
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect the staging area.' }

    # Stage all eligible changes, but never the known Visual Studio output paths or artifacts.
$AddPathspec = @(
    '.',
    ':!Project9/.vs', ':!Project9/.vs/**',
    ':!Project9/x64', ':!Project9/x64/**',
    ':!Project9/Project9/x64', ':!Project9/Project9/x64/**',
    ':!*.obj', ':!*.pdb', ':!*.ilk', ':!*.idb', ':!*.ipch',
    ':!*.tlog', ':!*.lastbuildstate', ':!*.exe.recipe'
)

    if ($WhatIf) {
        Write-BackupLog "WhatIf: would stage eligible changes and push branch '$Branch'."
        & git -C $RepositoryPath status --short --untracked-files=all
        exit 0
    }

& git -C $RepositoryPath add -A -- @AddPathspec
if ($LASTEXITCODE -ne 0) { throw "git add -A failed with exit code $LASTEXITCODE." }

    & git -C $RepositoryPath diff --cached --quiet
    if ($LASTEXITCODE -eq 0) {
        Write-BackupLog 'No eligible changes detected; no commit was created.'
        exit 0
    }
    if ($LASTEXITCODE -ne 1) { throw 'Could not inspect staged changes.' }

    Invoke-Git commit -m "Auto backup: $CommitDate"
    Invoke-Git push origin $Branch
    Write-BackupLog "Backup completed: pushed branch '$Branch'."
}
catch {
    Write-BackupLog "Backup failed: $($_.Exception.Message)"
    Write-Error $_
    exit 1
}
