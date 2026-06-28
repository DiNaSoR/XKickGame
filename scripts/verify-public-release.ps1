$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
Set-Location $root

$failed = $false

function Pass($message) {
    Write-Host "[PASS] $message"
}

function Fail($message) {
    Write-Host "[FAIL] $message" -ForegroundColor Red
    $script:failed = $true
}

function Warn($message) {
    Write-Host "[WARN] $message" -ForegroundColor Yellow
}

Write-Host "XKickGame public release verification"
Write-Host "Repository: $root"
Write-Host ""

$requiredFiles = @(
    "README.md",
    "README.ar.md",
    "LICENSE",
    "SECURITY.md",
    "CONTRIBUTING.md",
    "CODE_OF_CONDUCT.md",
    "THIRD_PARTY_NOTICES.md",
    "docker-compose.yml",
    ".env.example",
    ".gitattributes",
    ".github/workflows/public-release-check.yml",
    ".github/pull_request_template.md",
    ".github/ISSUE_TEMPLATE/bug_report.yml",
    ".github/ISSUE_TEMPLATE/setup_help.yml",
    "docs/DATABASE_SETUP.md",
    "docs/PUBLISHING.md",
    "docs/PUBLIC_RELEASE.md",
    "docs/RUNTIME_TROUBLESHOOTING.md",
    "server/runtime/mysql/00_local_user.sql",
    "server/runtime/mysql/bootstrap_minimal_xkick.sql",
    "server/runtime/mysql/seed_local_account.sql",
    "server/runtime/docker/Dockerfile"
)

$missing = $requiredFiles | Where-Object { -not (Test-Path -LiteralPath $_) }
if ($missing.Count -eq 0) {
    Pass "Required public release files exist."
} else {
    Fail "Missing required files: $($missing -join ', ')"
}

if (-not (Get-Command rg -ErrorAction SilentlyContinue)) {
    Fail "ripgrep (rg) is required for the secret scan."
} else {
    $secretPattern = 'H:\\|C:\\Users|kicks2|kicks2_log|mysql_personal|192\.168\.1\.121|DB_User\s*=\s*kicks|DB_Main\s*=\s*kicks2'
    $rgArgs = @(
        "-n", "-i", $secretPattern, ".",
        "-g", "!.git",
        "-g", "!scripts/verify-public-release.ps1",
        "-g", "!client/XKickOnline/*.pak",
        "-g", "!client/XKickOnline/*.bin",
        "-g", "!client/XKickOnline/*.exe",
        "-g", "!client/XKickOnline/*.dll",
        "-g", "!server/runtime/legacy-home/**/XKICK_*"
    )
    $secretOutput = & rg @rgArgs 2>&1
    $rgExit = $LASTEXITCODE
    if ($rgExit -eq 0) {
        Fail "Known private markers or machine-specific paths were found:`n$secretOutput"
    } elseif ($rgExit -eq 1) {
        Pass "Known private markers and machine-specific paths were not found."
    } else {
        Fail "Secret scan failed with exit code $rgExit.`n$secretOutput"
    }
}

$tracked = @(& git ls-files)
if ($LASTEXITCODE -ne 0) {
    Fail "git ls-files failed."
} else {
    $badTracked = $tracked | Where-Object {
        $_ -match '(^|/)\.svn/' -or
        $_ -match '^archive/logs/' -or
        $_ -match '^archive/packages/' -or
        $_ -match '^archive/legacy-home/' -or
        $_ -match '^archive/backup/XKICK_(Certify|Game)$'
    }
    if ($badTracked.Count -eq 0) {
        Pass "Tracked junk policy is clean."
    } else {
        Fail "Tracked junk files remain:`n$($badTracked -join [Environment]::NewLine)"
    }
}

$clientRuntime = @(& git ls-files "client/XKickOnline/**")
$serverRuntime = @(& git ls-files "server/runtime/legacy-home/**")
if ($clientRuntime.Count -gt 0 -and $serverRuntime.Count -gt 0) {
    Pass "Runtime is included ($($clientRuntime.Count) client files, $($serverRuntime.Count) server files)."
} else {
    Fail "Runtime files are missing from the tracked tree."
}

$overLimit = @()
$warnLarge = @()
foreach ($path in $tracked) {
    if (Test-Path -LiteralPath $path -PathType Leaf) {
        $item = Get-Item -LiteralPath $path
        if ($item.Length -ge 100MB) {
            $overLimit += "$path ($([Math]::Round($item.Length / 1MB, 2)) MiB)"
        } elseif ($item.Length -ge 50MB) {
            $warnLarge += "$path ($([Math]::Round($item.Length / 1MB, 2)) MiB)"
        }
    }
}

if ($overLimit.Count -eq 0) {
    Pass "No tracked file is over GitHub's 100 MiB hard limit."
} else {
    Fail "Tracked files over GitHub's 100 MiB hard limit:`n$($overLimit -join [Environment]::NewLine)"
}

if ($warnLarge.Count -gt 0) {
    Warn "Large tracked runtime files:`n$($warnLarge -join [Environment]::NewLine)"
}

if (Get-Command docker -ErrorAction SilentlyContinue) {
    $composeOutput = & docker compose config --quiet 2>&1
    if ($LASTEXITCODE -eq 0) {
        Pass "docker-compose.yml is valid."
    } else {
        Fail "docker compose config failed:`n$composeOutput"
    }
} else {
    Warn "Docker is not installed or not on PATH; skipped Compose validation."
}

if ($failed) {
    Write-Host ""
    Write-Host "Public release verification failed." -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Public release verification passed."
