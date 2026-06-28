$ErrorActionPreference = "Stop"

$installPath = $PSScriptRoot
$keys = @(
    "HKLM:\SOFTWARE\Gamepower7\XKickOnline",
    "HKLM:\SOFTWARE\WOW6432Node\Gamepower7\XKickOnline",
    "HKCU:\SOFTWARE\Gamepower7\XKickOnline",
    "HKCU:\SOFTWARE\WOW6432Node\Gamepower7\XKickOnline"
)

foreach ($key in $keys) {
    New-Item -Path $key -Force | Out-Null
    New-ItemProperty -Path $key -Name ProductDir -Value $installPath -PropertyType String -Force | Out-Null
    New-ItemProperty -Path $key -Name ProductExe -Value "XKick.bin" -PropertyType String -Force | Out-Null
    New-ItemProperty -Path $key -Name Version -Value "1.0.2.47" -PropertyType String -Force | Out-Null
}

Write-Host "XKick install registry keys written."
Start-Sleep -Seconds 2
