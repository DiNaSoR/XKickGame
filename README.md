# XKickGame

Legacy XKick Online client, server runtime, web tooling, and local database bootstrap.

This repository is prepared as a runtime-included public snapshot. The checked-in runtime artifacts are intentional because the recovered source tree is incomplete and the binaries are currently required for local play/testing.

Arabic instructions are available in [README.ar.md](README.ar.md).

## Project Status

These percentages describe practical readiness for local development. They are not a claim that every legacy feature is complete.

![Server Runtime](https://img.shields.io/badge/Server%20Runtime-90%25-success?style=for-the-badge)
![Server Source](https://img.shields.io/badge/Server%20Source-35%25-orange?style=for-the-badge)
![Client Runtime](https://img.shields.io/badge/Client%20Runtime-95%25-brightgreen?style=for-the-badge)
![Client Source](https://img.shields.io/badge/Client%20Source-45%25-yellow?style=for-the-badge)

| Area | Run-ready status | Source/rebuild status | Meaning |
| --- | ---: | ---: | --- |
| Server | 90% | 35% | The legacy server runtime, Docker wrapper, and MySQL bootstrap are included and documented. The recovered server source is incomplete, so the server currently runs from bundled legacy binaries. |
| Client | 95% | 45% | The Windows client runtime is included with local config and an install-registration helper. The legacy client source exists, but a clean rebuild path is not verified. |

In short: the repository is complete enough to run the local client/server stack, but it is not yet a fully rebuildable open-source codebase.

## What Is Included

- `client/XKickOnline/` - Windows client runtime, launcher, DLLs, PAK files, and local client helper scripts.
- `server/runtime/legacy-home/` - legacy Linux certify/game runtime binaries and runtime table files.
- `server/runtime/docker/` - Docker image wrapper for running the legacy 32-bit Linux binaries.
- `server/runtime/mysql/` - MySQL schema and seed data for local development.
- `web/` - legacy PHP admin and patch tooling.
- `data/` - extracted game table data.
- `docs/` - setup, publishing, and troubleshooting notes.

## Requirements

- Windows for the legacy game client.
- Docker Desktop with Docker Compose.
- PowerShell.
- `rg` / ripgrep for release verification.

## Quick Start

From the repository root:

```powershell
docker compose up --build
```

The Compose stack publishes ports `3306`, `2001`, and `3001`. Stop older local XKick/MySQL containers first if those ports are already in use.

If you need to recreate the local database:

```powershell
docker compose down -v
docker compose up --build
```

This deletes the Docker MySQL volume and reruns the SQL files in `server/runtime/mysql/`.

## Client Setup

The client is expected at:

```text
client/XKickOnline/
```

If the launcher reports `Not installed`, register the local client path from an elevated PowerShell:

```powershell
.\client\XKickOnline\register_xkick_install.ps1
```

Then start the launcher/client from `client/XKickOnline/`.

## Local Login

The database seed creates this local development account:

```text
username: test
password: 1234
```

These are public development credentials only. Do not reuse them on an internet-facing server.

## Public Release Check

Before publishing or opening a pull request, run:

```powershell
.\scripts\verify-public-release.ps1
```

The script checks required public files, known old private markers, tracked junk, runtime inclusion, GitHub file-size limits, and Docker Compose syntax.

## Publishing

Read [docs/PUBLISHING.md](docs/PUBLISHING.md) before making the repository public.

Recommended public release approach:

1. Run `.\scripts\verify-public-release.ps1`.
2. Confirm redistribution rights for the bundled client/runtime files.
3. Publish from a clean Git history if old private values may exist in local history.
4. Push to a new empty GitHub repository.

## Repository Layout

| Path | Purpose |
| --- | --- |
| `client/` | Legacy Windows client runtime and client/shared source. |
| `server/` | Legacy server source, runtime binaries, Docker runtime wrapper, and MySQL bootstrap SQL. |
| `web/` | Legacy PHP admin and patch tooling. |
| `data/` | Extracted game table data. |
| `archive/` | Historical table backup material that is not part of the active runtime. |
| `docs/` | Setup notes, runtime troubleshooting, and public-release tracking. |
| `scripts/` | Local verification and maintenance scripts. |

## Documentation

- [docs/DATABASE_SETUP.md](docs/DATABASE_SETUP.md) - Docker MySQL bootstrap and local credentials.
- [docs/PUBLISHING.md](docs/PUBLISHING.md) - exact publication workflow.
- [docs/PROJECT_LAYOUT.md](docs/PROJECT_LAYOUT.md) - reorganized folder map.
- [docs/PUBLIC_RELEASE.md](docs/PUBLIC_RELEASE.md) - public-release policy and checklist.
- [docs/RUNTIME_TROUBLESHOOTING.md](docs/RUNTIME_TROUBLESHOOTING.md) - runtime failures and fixes already found.
- [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) - bundled legacy/runtime artifact policy.

## Important Notice

The MIT License applies to source code, scripts, documentation, and new project files authored for this repository unless a file says otherwise. Bundled legacy client/runtime binaries, assets, DLLs, PAK files, art, audio, fonts, and media may be subject to separate rights.

Verify redistribution rights before making the repository public.

## License

MIT License. See [LICENSE](LICENSE).
