# Runtime Troubleshooting Log

This document tracks local XKick runtime problems found during the Windows client plus Docker/MySQL server setup.

Do not store local root passwords here. Use environment-specific notes outside the repo for private credentials.

## Current Local Runtime

| Area | Current value |
| --- | --- |
| Installed client | `client/XKickOnline/` |
| Client config | `client/XKickOnline/GameConfig.conf` |
| Server runtime | `server/runtime/legacy-home/` |
| MySQL service | `mysql` from `docker-compose.yml` |
| Runtime service | `runtime` from `docker-compose.yml`, TCP/UDP `2001`, TCP/UDP `3001` |
| Runtime image | Built from `server/runtime/docker/Dockerfile` |
| Runtime DB user | `xkick` |
| Local test game login | `test` / `1234` |
| Seed SQL | `server/runtime/mysql/seed_local_account.sql` |
| Bootstrap SQL | `server/runtime/mysql/bootstrap_minimal_xkick.sql` |

## Active Known Issue

### Players List Client Crash

Status: open / under investigation.

Symptom:
- Opening the players/buddy list crashes `XKick.bin`.
- Crash reports:
  - `client/XKickOnline/Crashed_Report/XKick 2026-06-24 01,49,34.TXT`
  - `client/XKickOnline/Crashed_Report/XKick 2026-06-24 01,54,25.TXT`
  - `client/XKickOnline/Crashed_Report/XKick 2026-06-24 02,05,30.TXT`

Evidence:
- All reports fault at `00423B35` with `C0000005 (ACCESS_VIOLATION)`.
- Disassembly shows the fault while dereferencing a UI element for `Position_Icon`.
- The server logs show the crash occurs after `CM_BUDDY_INFO(2316)` returns successfully.

Attempted mitigation:
- Seeded valid local buddy rows in `tb_game_buddy`.
- Added seed logic in `server/runtime/mysql/seed_local_account.sql`.
- Verified `tb_game_buddy` has 6 rows and target positions are valid (`10`, `40`).

Result:
- A later crash report still appeared at `02:05:30`, so the buddy-row seed did not fully solve the crash.

Next investigation path:
- Patch or guard the client-side buddy/player list renderer.
- Source reference: `client/source/legacy-game-client/GetPacket.cpp`, `GetBuddyInfo`.
- Source reference: `client/source/legacy-game-client/PlayerInformationDialog.cpp`, `FUNCTION_AtheletListInformation`.
- Binary fault location: `client/XKickOnline/XKick.bin`, VA `0x00423B35`.

## Fixed / Mitigated Issues

### Launcher Shows Webpage Canceled

Symptom:
- Launcher embedded browser showed "Navigation to the webpage was canceled".
- Patch progress did not complete reliably.

Cause:
- The legacy patch URL was not available locally.

Fix:
- Added local patch manifest: `client/XKickOnline/local_patch/xkick_du.xml`.
- Pointed `client/XKickOnline/XKickPatch.ini` to `http://127.0.0.1:8787/xkick_du.xml`.
- Started a local Python HTTP server for the manifest on `127.0.0.1:8787`.

Verification:
- `Invoke-WebRequest -UseBasicParsing http://127.0.0.1:8787/xkick_du.xml` returned the local manifest.

### Launcher Says "Not Installed"

Symptom:
- Clicking Start in the launcher showed `XKick: Not installed`.

Cause:
- The installed client lacked the legacy Windows registry install keys expected by the launcher.

Fix:
- Added and ran `client/XKickOnline/register_xkick_install.ps1`.

Verification:
- Launcher stopped showing `Not installed`.

### CreateProcess Failed 740

Symptom:
- Launcher showed `CreateProcess failed (740)`.

Cause:
- Windows elevation was required for this legacy launcher/game path.

Fix / workaround:
- Run the launcher/game with administrator rights when Windows requires elevation.

Verification:
- After elevation and registry setup, the game executable launched.

### Game Crashed Immediately After Launch

Symptom:
- `XKick.bin` opened and then crashed before login.

Cause:
- The client queried Direct3D framebuffer format `R5G6B5`, which modern drivers did not support in this environment.

Fix:
- Backed up original binary as `client/XKickOnline/XKick.bin.orig-r5g6b5`.
- Patched `client/XKickOnline/XKick.bin` at file offset `0x5170`, changing bytes `6A 04` to `6A 03`.
- This changes the framebuffer format enum from `R5G6B5` to `X8R8G8B8`.

Verification:
- The client reached the Arabic login screen.

### Window Size / Windowed Mode

Symptom:
- User needed game settings such as windowed mode.

Fix:
- Set `client/XKickOnline/GameConfig.conf` to:
  - `Full="false"`
  - `Wide="false"`

Verification:
- Client launched in a window.

### Unknown Login Credentials

Symptom:
- Login screen was reachable, but no account was known.

Fix:
- Seeded a local test account in `server/runtime/mysql/seed_local_account.sql`.
- Login: `test` / `1234`.
- Seeded local test characters including `[FW]test`, `[GK]test`, and later `Tesr`.

Verification:
- Login succeeded and character selection became reachable.

### Server List Blank

Symptom:
- Server list page appeared but showed no selectable rows.

Cause:
- `tb_game_server.server_state = 0` means `SERVER_STATE_EMPTY`.
- Client filters out empty server rows before rendering.

Fix:
- Set local game rows to running state `2`.
- Added two channel-1 rows so the client has the two visible entries it expects:
  - `01-00 Local Game`
  - `01-01 Game 1`
- Kept Certify on channel `0` so it is not selected as a game server.
- Persisted in `server/runtime/mysql/bootstrap_minimal_xkick.sql`.

Verification:
- Server list rendered rows again.
- Certify logs showed `CM_SERVER_LIST(1300)` returning `Req(0)`.

### Server Not Working Error `130114`

Symptom:
- Selecting a server showed Arabic "server is not working now" with error code `130114`.

Cause:
- The client was selecting a checkup/certify-style row or a row not marked running.
- Log mapping: `CM_CHOICE_SERVER(1301)` returned `Req(-14)`.

Fix:
- Marked playable rows as state `2` (`SERVER_STATE_RUN`).
- Removed Certify from the channel-1 game server list.
- Ensured `01-01 Game 1` points to `127.0.0.1:3001`.

Verification:
- Certify logs later showed `CM_CHOICE_SERVER(1301)` returning `Req(0)`.

### Character Creation Error `120211`

Symptom:
- Creating a character showed error code `120211`.

Cause:
- Server log: `CM_CREATE_CHARACTER(1202)` returned `Req(-11)`.
- Database log showed insert failure:
  - `INSERT INTO tb_game_player (player_seq, ...) VALUES ('', ...)`
- MySQL 8 strict mode rejected legacy `''` into an auto-increment integer column.

Fix:
- Set MySQL SQL mode to `NO_ENGINE_SUBSTITUTION`.
- Persisted the mode with MySQL `SET PERSIST`.

Verification:
- Fresh DB session showed `@@SESSION.sql_mode = NO_ENGINE_SUBSTITUTION`.
- A temporary test table accepted the legacy `id=''` auto-increment insert pattern.
- `CM_CREATE_CHARACTER(1202)` later returned `Req(0)`.

### Game Login Error `200014`

Symptom:
- Entering the game server showed error code `200014`.

Cause:
- Server log: `CM_GAME_LOGIN(2000)` returned `Req(-14)`.
- Database log showed missing monthly record shard tables:
  - `tb_game_record_26_02`
  - `tb_game_record_02_23`

Fix:
- Added compatibility tables:
  - `tb_game_record_26_02`
  - `tb_game_record_02_23`
  - `tb_game_record_26_06`
- Copied seeded records into the shards.
- Persisted in `server/runtime/mysql/bootstrap_minimal_xkick.sql` and `server/runtime/mysql/seed_local_account.sql`.

Verification:
- Game server login later returned `CM_GAME_LOGIN(2000) Req(0)`.

### Missing Rank / Sale Shard Tables

Symptom:
- After successful game login, the client crashed during early player-info panels.

Cause:
- Database log showed missing compatibility shards:
  - `tb_game_rank_26_02`
  - `tb_log_sale_26_02`
  - `tb_log_sale_02_23`

Fix:
- Added rank shard tables:
  - `tb_game_rank_26_02`
  - `tb_game_rank_02_23`
  - `tb_game_rank_26_06`
- Added sale log shard tables:
  - `tb_log_sale_26_02`
  - `tb_log_sale_02_23`
- Added default rank rows for all local players.
- Persisted in bootstrap and seed SQL.

Verification:
- `tb_game_rank_26_02` had rows for players `1`, `3`, and `4`.
- `tb_log_sale_26_02` and `tb_log_sale_02_23` existed.
- No new DB errors appeared for these tables in later `Game101` logs.

### Create Match Error `220517`

Symptom:
- Creating a match showed Arabic "error in transfer settings" with error code `220517`, then the game could close.

Cause:
- Packet log showed `CM_CREATE_ROOM(2203)` returned `Req(0)`, then `CM_CHOICE_ROOM(2205)` returned `Req(-17)`.
- Error code `220517` maps to `CM_CHOICE_ROOM(2205)` plus result `-17`.
- `Default_Eng/Table_Msg.csv` maps this result to "Your level doesn't meet the requirement of the room".
- Server binary disassembly showed `Req(-17)` comes from `CRoom::InsertPlayerRoom` return `-7`, which is the room level-range check.
- The installed client sends `CM_CREATE_ROOM` with body size `79`, while the server-side `CCSCreateRoom` constructor expects body size `0x50` (`80`).
- Because of the one-byte layout mismatch, `CreateRoom` read the room level/check/max/position fields one byte too late. The room was created, but its level range was invalid, so the owner was rejected during the automatic `CM_CHOICE_ROOM`.

Fix:
- Added `server/runtime/tools/patch_create_room_layout.py`.
- Patched the runtime game-server binaries so `CreateRoom` reads the installed client's 79-byte create-room layout:
  - `server/runtime/legacy-home/XKICK_Game`
  - `server/runtime/legacy-home/game1/XKICK_Game1`
  - `server/runtime/legacy-home/game2/XKICK_Game2`
  - `server/runtime/legacy-home/game3/XKICK_Game3`
- The patch shifts the post-point-code reads for level, check flags, max count, and home/away positions one byte earlier.
- Backups were created next to each patched binary with `.orig-create-room-layout` suffix.
- Restarted `xkick_game1` after patching.

Verification:
- `patch_create_room_layout.py` reports all runtime binaries as `already patched`.
- Byte verification on `game1/XKICK_Game1` shows the patched offsets at `0x809CE20`, `0x809CE2C`, `0x809CE38`, `0x809CE44`, `0x809CE6A`, `0x809CE76`, `0x809CE82`, `0x809CE8E`, `0x809CEAA`, and `0x809CEC5`.
- `xkick_game1` is running after restart.
- `Test-NetConnection 127.0.0.1 -Port 3001` returned `TcpTestSucceeded: True`.
- Later packet logs confirmed `CM_CHOICE_ROOM(2205)` returned `Req(0)` after `CM_CREATE_ROOM(2203)`.

### Create Room Client Crash After Room Accepted

Symptom:
- Creating a room no longer shows `220517`, but `XKick.bin` crashes immediately after the ready-room load starts.

Evidence:
- Latest crash report:
  - `client/XKickOnline/Crashed_Report/XKick 2026-06-26 01,29,33.TXT`
- Fault address was `0059B5E0` with `C0000005 (ACCESS_VIOLATION)` and `EAX=00000000`.
- Packet log showed the server accepted the room flow before the client exited:
  - `CM_CREATE_ROOM(2203) Req(0)`
  - `CM_CHANGE_JANG(2302) Req(0)`
  - `CM_CHOICE_ROOM(2205) Req(0)`
  - `CM_ROOM_INFO(2200) Req(0)`
  - `CM_ATHLETE_INFO(2303) Req(0)`
  - `CM_ATHLETE_END(2304) Req(0)`
- Client disassembly showed the crash inside `UIFunction::ChangeChildTexture`, dereferencing the result of a missing UI control lookup.
- Caller `0x004709B0` matches `CReadyRoomProcess::SetUIReadyRoomPositionSetting()`, which formats and loads controls such as `RPlayerPosition_H_%d`, `SetPositionBtn%d`, `RPositionBtn_%d`, and `BPositionBtn_%d`.
- Replacing the wide ready-room UI fixed one incompatible UI entry, but the client still crashed on a later retry at the same `0059B5E0` address. That means at least one server-provided ready-room position byte can still format a texture name that the UI palette does not contain, such as a shifted or otherwise invalid `RPositionBtn_%d` / `BPositionBtn_%d` value.

Cause:
- `client/XKickOnline/Ui.pak` contained an incompatible `Process_W_ReadyRoom.ui`.
- That wide ready-room entry did not contain the controls required by this client binary, including `ReadyRoomMain`, `RPlayerPosition_H_0`, `SetPositionBtn4`, `SetPositionBtn10`, `RPositionBtn_0`, and `BPositionBtn_0`.
- The working normal ready-room entry, `Process_N_ReadyRoom_New.ui`, contained all required controls.
- If the client started in wide mode or loaded the wide ready-room UI, the missing controls caused a null dereference during ready-room position icon setup.
- The remaining crash is contained as a bad ready-room position palette lookup. Root cause still needs packet capture if it reappears: decode `CM_ROOM_INFO(2200)` and compare the home/away reserve-position bytes against the expected values `[10, 20, 30, 1, 0, 40]`.

Fix:
- Added repeatable patch helper:
  - `server/runtime/tools/patch_client_readyroom_ui.py`
- Added containment patch helper:
  - `server/runtime/tools/patch_client_readyroom_position_aliases.py`
- Patched the installed client:
  - `python server/runtime/tools/patch_client_readyroom_ui.py client/XKickOnline/Ui.pak`
  - `python server/runtime/tools/patch_client_readyroom_position_aliases.py client/XKickOnline/Ui.pak`
- The helper rebuilt `Ui.pak` with `Process_W_ReadyRoom.ui` replaced by the working `Process_N_ReadyRoom_New.ui` payload.
- The alias helper cloned the ready-room `RPositionBtn_0` and `BPositionBtn_0` controls for every signed byte value from `-128` through `255` in both `Process_W_ReadyRoom.ui` and `Process_N_ReadyRoom_New.ui`. This prevents `UIFunction::ChangeChildTexture` from receiving a null source texture control if the server sends a bad or shifted position byte.
- Backup created:
  - `client/XKickOnline/Ui.pak.orig-readyroom-ui`
  - `client/XKickOnline/Ui.pak.orig-readyroom-position-aliases`

Verification:
- Patched `Process_W_ReadyRoom.ui` now has the same required ready-room controls as `Process_N_ReadyRoom_New.ui`, including `ReadyRoomMain`, `RPlayerPosition_H_0`, `RPlayerPosition_A_0`, `SetPositionBtn4`, `SetPositionBtn10`, `RPositionBtn_0`, `BPositionBtn_0`, `GameStart`, `ScaleIcon`, `MapImg`, and `BallImg`.
- Alias verification on `Process_W_ReadyRoom.ui` shows `384` `RPositionBtn_*` names and `384` `BPositionBtn_*` names, including `RPositionBtn_4`, `BPositionBtn_4`, `RPositionBtn_-1`, and `BPositionBtn_255`.
- The temporary debug proxy was stopped and `tb_game_server.server_port` was restored to `3001` for server codes `100` and `101`.
- `xkick_certify` and `xkick_game1` were already running.
- `Test-NetConnection 127.0.0.1 -Port 2001` returned `TcpTestSucceeded: True`.
- `Test-NetConnection 127.0.0.1 -Port 3001` returned `TcpTestSucceeded: True`.
- Manual in-client retry is still required to confirm the ready-room screen remains open after creating a room. If it still crashes, capture the latest crash report and run the packet debug proxy again to decode `CM_ROOM_INFO`.

### Character Select Crash After `CM_CHOICE_CHARACTER`

Symptom:
- Choosing a character crashed `XKick.bin`.
- The server did not reject the request.

Cause:
- Crash reports:
  - `client/XKickOnline/Crashed_Report/XKick 2026-06-24 02,21,30.TXT`
  - `client/XKickOnline/Crashed_Report/XKick 2026-06-24 02,28,38.TXT`
- Both reports faulted at `004320C8` with `C0000005`.
- Certify packet log showed:
  - `CM_CHOICE_CHARACTER(1204) Req(0)`
  - no following `CM_SERVER_LIST(1300)` request
- Disassembly at `004320C8` showed a null dereference while drawing the common experience UI.
- Root cause was the earlier level `60` local-player workaround. The installed client `Table.pak` only has `Table_Level.csv` rows for levels `1..50`, so level `60` made the client look up a missing level resource.

Fix:
- Changed the live DB local characters from level `60` to level `50`.
- Set `player_exp = 1559074`, matching the level `50` target EXP in the client level table.
- Updated `server/runtime/mysql/seed_local_account.sql` to persist level `50` instead of level `60`.

Verification:
- Extracted `Table_Level.csv` from `client/XKickOnline/Table.pak`; it has `50` rows, max level `50`.
- Reran `server/runtime/mysql/seed_local_account.sql`.
- Live DB query returned all local test characters at level `50` with EXP `1559074`.

### Store Button Client Crash

Symptom:
- Clicking the store button crashed `XKick.bin`.

Cause:
- Client crash report:
  - `client/XKickOnline/Crashed_Report/XKick 2026-06-24 02,06,02.TXT`
- Fault address was `0044A339`.
- Packet log showed the crash immediately after:
  - `CM_RANDOMSHOPITEM_LIST(3111)`
  - server response `Req(0)`
- The mounted server item tables contained item codes that were not present in the installed client's packed `Table.pak`.
- The client store renderer dereferenced the missing item resource instead of guarding it.

Fix:
- Backed up the previous mounted item tables in:
  - `archive/runtime-table-backups/2026-06-24-client-compatible-item-tables/`
- Added repeatable helper:
  - `server/runtime/tools/extract_client_table_pak.py`
- Regenerated `data/game-tables/*/Table_Item.csv` from the original server-format tables, filtered to the item codes present in `client/XKickOnline/Table.pak`.
- The helper also renumbers the first `Index` column after filtering because the legacy table loader rejects index gaps.

Verification:
- Generator check passed:
  - `python server/runtime/tools/extract_client_table_pak.py --pak client/XKickOnline/Table.pak --server-source archive/runtime-table-backups/2026-06-24-client-compatible-item-tables --out data/game-tables --check`
- Filtered mounted tables now contain `174` client-compatible item codes per language table.
- `xkick_game1` restarted and stayed running.
- `Test-NetConnection 127.0.0.1 -Port 3001` returned `TcpTestSucceeded: True`.
- `XKICK_Game1` process is running inside the `xkick_game1` container.

## Useful Log Locations

| Purpose | Path |
| --- | --- |
| Certify packet log | `server/runtime/legacy-home/Log/PACKET/Certify1__2026_06_23.log` |
| Game packet log | `server/runtime/legacy-home/Log/PACKET/Game101__2026_06_23.log` |
| Game DB errors | `server/runtime/legacy-home/Log/DATABASE/Game101__2026_06_23.log` |
| Game runtime errors | `server/runtime/legacy-home/Log/ERROR/Game101__2026_06_23.log` |
| Client crash reports | `client/XKickOnline/Crashed_Report/` |

## Useful Verification Commands

```powershell
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}"
Test-NetConnection 127.0.0.1 -Port 2001
Test-NetConnection 127.0.0.1 -Port 3001
docker compose exec mysql mysql -uroot -p -e "SHOW GLOBAL VARIABLES LIKE 'sql_mode';"
```

Use the MySQL password interactively for the final command. Do not write it into docs.

## Runtime Restart Commands

```powershell
docker compose restart runtime
```

If containers need to be recreated:

```powershell
docker compose down
docker compose up --build
```
