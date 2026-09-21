[CmdletBinding()]
param(
    [ValidatePattern('^([01]\d|2[0-3]):[0-5]\d$')]
    [string]$Time = '23:30'
)

$ErrorActionPreference = 'Stop'
$RepositoryPath = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$BackupScript = Join-Path $PSScriptRoot 'Backup-GitHub.ps1'
$TaskName = 'Project9 Daily GitHub Backup'
$PowerShellExe = (Get-Command powershell.exe -ErrorAction Stop).Source
$TaskAction = New-ScheduledTaskAction -Execute $PowerShellExe -Argument ('-NoProfile -NonInteractive -ExecutionPolicy Bypass -File "{0}"' -f $BackupScript)
$TaskTrigger = New-ScheduledTaskTrigger -Daily -At $Time
$TaskUser = (Get-Acl -LiteralPath $RepositoryPath).Owner
$TaskPrincipal = New-ScheduledTaskPrincipal -UserId $TaskUser -LogonType Interactive -RunLevel Limited
$TaskSettings = New-ScheduledTaskSettingsSet -StartWhenAvailable -ExecutionTimeLimit (New-TimeSpan -Minutes 30) -MultipleInstances IgnoreNew

Register-ScheduledTask -TaskName $TaskName -Action $TaskAction -Trigger $TaskTrigger -Principal $TaskPrincipal -Settings $TaskSettings -Description "Checks $RepositoryPath daily and commits/pushes eligible changes only." -Force | Out-Null
Write-Host "Installed '$TaskName' for $Time daily."
Write-Host "Test without committing: powershell -NoProfile -ExecutionPolicy Bypass -File `"$BackupScript`" -WhatIf"
