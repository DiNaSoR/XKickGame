# Database Setup

The local development database is designed to run in Docker. It uses public default credentials so a fresh clone can boot without private secrets.

## Docker Compose

From the repository root:

```powershell
docker compose up --build
```

The stack publishes ports `3306`, `2001`, and `3001`. Stop older local XKick/MySQL containers first if those ports are already in use.

Compose starts:

- `mysql`: MySQL 5.7 with schema/seed files from `server/runtime/mysql/`.
- `runtime`: the legacy Linux runtime mounted from `server/runtime/legacy-home/`.

The MySQL volume is named `xkick-mysql-data`. SQL files in `server/runtime/mysql/` run only when that volume is first created.

## Public Local Credentials

The runtime config uses:

```text
host: mysql
database: xkick
log database: xkick_log
sample database: xkick_sample
user: xkick
password: xkick_dev_password
```

The seeded game account is:

```text
username: test
password: 1234
```

These are local-development defaults only. Change them before any public deployment.

## Recreating The Database

To rebuild the database from the bootstrap SQL:

```powershell
docker compose down -v
docker compose up --build
```

This deletes the local Docker database volume.

## SQL Files

| File | Purpose |
| --- | --- |
| `server/runtime/mysql/00_local_user.sql` | Creates the local `xkick` MySQL user and grants access to the development databases. |
| `server/runtime/mysql/bootstrap_minimal_xkick.sql` | Creates the minimum schema and data needed for the legacy runtime to start. |
| `server/runtime/mysql/seed_local_account.sql` | Creates the `test` account and starter players used by the local client. |

If the runtime password changes, update both `00_local_user.sql` and the runtime config files under `server/runtime/legacy-home/`.
