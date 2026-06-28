# Public Release Notes

This document tracks the repository policy for making the legacy snapshot safe and understandable as an open-source project.

## Runtime Policy

Runtime artifacts are intentionally included:

- `client/XKickOnline/`
- `server/runtime/legacy-home/`
- `server/runtime/docker/`
- `server/runtime/mysql/`

The current server source snapshot is incomplete, so these artifacts are needed for local play and debugging.

## Status Summary

| Area | Run-ready status | Source/rebuild status | Notes |
| --- | ---: | ---: | --- |
| Server | 90% | 35% | Runnable through bundled legacy binaries, Docker, and MySQL seed data. Missing source files prevent a verified rebuild. |
| Client | 95% | 45% | Windows runtime is included and configured. Legacy client source exists, but full rebuild instructions are not verified. |

This means the public repository is ready for local runtime testing, not yet for fully source-built releases.

## Cleaned For Public Use

- Private database passwords were replaced with public local-development defaults.
- PHP admin/patch database settings now read from environment variables where practical.
- Legacy `.svn` metadata was removed from the tracked tree.
- Historical runtime logs, shell history files, and package tarballs were removed from the tracked tree.
- Runtime logs and local binary backup files are ignored.

## Still Needs Review

- Confirm redistribution rights for the bundled legacy client, runtime binaries, DLLs, PAK files, and media assets.
- Decide whether to rewrite Git history before publishing if old private values were ever committed.
- Replace public development passwords before any internet-facing deployment.
- Recover or rebuild the missing server source files listed in `docs/PROJECT_LAYOUT.md`.

## Release Verification

Run this before publishing:

```powershell
.\scripts\verify-public-release.ps1
```

See `docs/PUBLISHING.md` for the recommended clean-history publication flow.

## Public Defaults

The repository includes default local credentials so contributors can run the game locally:

```text
MySQL user: xkick
MySQL password: xkick_dev_password
Game account: test / 1234
```

These are not secrets. They exist only for local development.
