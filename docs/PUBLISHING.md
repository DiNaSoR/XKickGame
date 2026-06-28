# Publishing Guide

This repository is prepared for a runtime-included public release. The engineering checks can be run locally or in GitHub Actions.

## Before Publishing

Run:

```powershell
.\scripts\verify-public-release.ps1
```

The verifier checks:

- Required public release files exist.
- Known old private markers and machine-specific paths are absent from current files.
- Legacy `.svn`, archive logs, shell history, and duplicate archive binaries are not tracked.
- Client and server runtime artifacts are still included.
- No tracked file is over GitHub's 100 MiB hard limit.
- `docker-compose.yml` is valid.

## Recommended Public History

If old secrets or private logs may exist in Git history, publish from a clean history instead of pushing the existing local history.

Recommended clean-history flow:

```powershell
cd ..
robocopy XKickGame XKickGame-public /MIR /XD .git
cd XKickGame-public
git init
git add -A
git commit -m "Initial public release"
git branch -M main
git remote add origin https://github.com/<owner>/XKickGame.git
git push -u origin main
```

Create the GitHub repository empty first, without adding a README, license, or `.gitignore` in the GitHub UI.

## Existing-History Flow

Only use this if you are sure the existing Git history is acceptable for public release:

```powershell
.\scripts\verify-public-release.ps1
git add -A
git commit -m "Prepare public release"
git remote add origin https://github.com/<owner>/XKickGame.git
git branch -M main
git push -u origin main
```

## Owner Checks

Before switching the GitHub repository to public, the owner should confirm:

- Redistribution of `client/XKickOnline/` is allowed.
- Redistribution of `server/runtime/legacy-home/` is allowed.
- Bundled DLLs, PAK files, art, audio, fonts, and media assets may be included publicly.
- Public development credentials are acceptable because the repo is for local development only.
- Any internet-facing deployment uses new private credentials.

## After Publishing

After the first public push:

```powershell
git clone https://github.com/<owner>/XKickGame.git XKickGame-smoke-test
cd XKickGame-smoke-test
.\scripts\verify-public-release.ps1
docker compose up --build
```

If ports `3306`, `2001`, or `3001` are already in use, stop the old local containers first or change the published ports in `docker-compose.yml`.
