# Security Policy

This is a legacy game/server snapshot intended for local development and preservation work.

Do not expose the Docker Compose runtime directly to the internet. The included credentials are public development defaults, not production secrets.

## Reporting Issues

Open a private security advisory or contact the maintainer before posting exploitable details publicly.

## Known Risks

- Legacy PHP code uses old `mysql_*` APIs.
- The runtime uses legacy binary server components.
- The database bootstrap includes public local credentials.
- Historical Git history may still contain values that were removed from the current working tree.
