# Project Layout

This repository is a legacy production/source snapshot. The current structure keeps client, server, web, data, and historical artifacts separated while preserving original files.

## Top-Level Areas

| Path | Purpose |
| --- | --- |
| `client/source/legacy-game-client/` | Legacy client/shared C++ project previously under `KicksServer/game/common/CLIENT_SHARE`. |
| `server/source/kicks/` | Legacy Linux server source tree, including `certify`, `game`, `common`, and `Table`. |
| `server/runtime/legacy-home/` | Runtime/deployment-style files that were previously at the repository root, including copied server binaries and game instance folders. |
| `server/runtime/docker/` | Docker image wrapper for running the legacy 32-bit Linux binaries on a modern host. |
| `server/runtime/mysql/` | Local MySQL bootstrap SQL for the Docker development database. |
| `server/source/legacy-root/` | Legacy root `Makefile` preserved separately from the active server source tree. |
| `web/admin/production/` | Former `xkickadmin` PHP admin site. |
| `web/admin/test/` | Former `admintest.xkick` PHP admin/test site. |
| `web/patch/` | Legacy patch web tooling. |
| `web/config/` | Web server configuration, including `httpd.conf`. |
| `web/tools/` | Small web utility scripts such as `phpinfo.php`. |
| `data/game-tables/` | Root-level game data tables. |
| `archive/backup/` | Former `Backup` table snapshot retained as historical reference. Duplicate runtime binaries were removed from this archive. |

## Large Local Downloads

The old admin download folders were moved with the web apps and remain ignored by Git:

- `web/admin/test/download/`
- `web/admin/production/download/`

These are large installer/media payloads and are intentionally kept out of version control.

## Runtime Artifacts

The repository intentionally tracks the Windows client runtime under `client/XKickOnline/` and the Linux server runtime under `server/runtime/legacy-home/`. Generated logs and local binary backup files beside those artifacts are ignored.

## Server Source Caveat

The Makefiles in `server/source/kicks/game/` and `server/source/kicks/certify/` still reference many per-server C++ files that are not present in this repository snapshot, including `main.cpp`, `server.cpp`, `packet.cpp`, `config.cpp`, `thread.cpp`, and `sql.cpp`.

The common C++ source is now in `server/source/kicks/common/`, matching the `../common` paths used by those Makefiles. The missing per-server files still need to be recovered or the binaries in `server/runtime/legacy-home/` must be treated as the only available server implementation.
