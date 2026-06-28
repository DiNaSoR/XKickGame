#!/usr/bin/env python3
"""Extract decoded CSV tables from the installed client's Table.pak.

The legacy pack format used by this client stores:
  - uint32 file count
  - repeated entries: uint32(name_len + 1), name bytes, uint32 offset, uint32 size
  - data block, with each byte XORed by 0xff

This is intentionally narrow and only extracts the item tables needed by the
local server runtime to keep random shop item codes compatible with the client.
"""

from __future__ import annotations

import argparse
import struct
from pathlib import Path


ITEM_TABLES = {
    "Default_Eng/Table_Item.csv": "Default_Eng_Table_Item.csv",
    "Dubai_Arb/Table_Item.csv": "Dubai_Arb_Table_Item.csv",
    "Korean/Table_Item.csv": "Korean_Table_Item.csv",
}


def read_entries(pack_bytes: bytes) -> tuple[int, dict[str, tuple[int, int]]]:
    pos = 0
    (count,) = struct.unpack_from("<I", pack_bytes, pos)
    pos += 4

    entries: dict[str, tuple[int, int]] = {}
    for _ in range(count):
        (stored_name_len,) = struct.unpack_from("<I", pack_bytes, pos)
        pos += 4

        name_len = stored_name_len - 1
        name = pack_bytes[pos : pos + name_len].decode("utf-8")
        pos += name_len

        offset, size = struct.unpack_from("<II", pack_bytes, pos)
        pos += 8
        entries[name] = (offset, size)

    return pos, entries


def extract_table(pack_bytes: bytes, data_start: int, offset: int, size: int) -> bytes:
    encoded = pack_bytes[data_start + offset : data_start + offset + size]
    return bytes(byte ^ 0xFF for byte in encoded)


def row_code(row: bytes) -> str | None:
    parts = row.rstrip(b"\r\n").split(b",", 2)
    if len(parts) < 3 or not parts[1].isdigit():
        return None
    return parts[1].decode("ascii")


def ordered_codes(table_bytes: bytes) -> list[str]:
    codes: list[str] = []
    seen: set[str] = set()
    for row in table_bytes.splitlines(keepends=True)[1:]:
        code = row_code(row)
        if code is not None and code not in seen:
            codes.append(code)
            seen.add(code)
    return codes


def read_server_rows(source: Path) -> tuple[bytes, dict[str, bytes]]:
    rows = source.read_bytes().splitlines(keepends=True)
    if not rows:
        raise SystemExit(f"empty server table: {source}")

    by_code: dict[str, bytes] = {}
    for row in rows[1:]:
        code = row_code(row)
        if code is not None:
            by_code[code] = row

    return rows[0], by_code


def reindex_row(row: bytes, index: int) -> bytes:
    parts = row.split(b",", 1)
    if len(parts) != 2:
        return row
    return str(index).encode("ascii") + b"," + parts[1]


def write_or_check(output: Path, content: bytes, check: bool) -> bool:
    if check:
        if output.exists() and output.read_bytes() == content:
            print(f"ok {output}")
            return True
        print(f"would update {output}")
        return False

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(content)
    print(f"wrote {output} ({len(content)} bytes)")
    return True


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--pak", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument(
        "--server-source",
        type=Path,
        help=(
            "Directory containing server-format *_Table_Item.csv backups. "
            "When set, output tables keep the server schema and are filtered "
            "to the item codes present in the installed client Table.pak."
        ),
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Verify output files already match the generated content.",
    )
    args = parser.parse_args()

    pack_bytes = args.pak.read_bytes()
    data_start, entries = read_entries(pack_bytes)
    ok = True

    server_headers: dict[str, bytes] = {}
    server_rows: dict[str, dict[str, bytes]] = {}
    fallback_rows: dict[str, bytes] = {}
    if args.server_source is not None:
        for source_name in ITEM_TABLES.values():
            header, rows = read_server_rows(args.server_source / source_name)
            server_headers[source_name] = header
            server_rows[source_name] = rows
            fallback_rows.update(rows)

    for table_name, source_name in ITEM_TABLES.items():
        if table_name not in entries:
            raise SystemExit(f"missing {table_name} in {args.pak}")

        offset, size = entries[table_name]
        client_table = extract_table(pack_bytes, data_start, offset, size)

        if args.server_source is None:
            content = client_table
        else:
            rows = server_rows[source_name]
            missing: list[str] = []
            output_rows = [server_headers[source_name]]
            for index, code in enumerate(ordered_codes(client_table)):
                row = rows.get(code) or fallback_rows.get(code)
                if row is None:
                    missing.append(code)
                    continue
                output_rows.append(reindex_row(row, index))

            if missing:
                raise SystemExit(
                    f"{source_name} is missing {len(missing)} client item codes: "
                    f"{', '.join(missing[:10])}"
                )
            content = b"".join(output_rows)

        ok = write_or_check(args.out / table_name, content, args.check) and ok

    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
