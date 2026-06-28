from __future__ import annotations

import argparse
import re
import shutil
import struct
from pathlib import Path


TARGET_ENTRIES = ("Process_W_ReadyRoom.ui", "Process_N_ReadyRoom_New.ui")
BACKUP_SUFFIX = ".orig-readyroom-position-aliases"
ALIAS_RANGE = range(-128, 256)


def read_entries(pack_bytes: bytes) -> tuple[int, list[tuple[str, int, int]]]:
    pos = 0
    (count,) = struct.unpack_from("<I", pack_bytes, pos)
    pos += 4

    entries: list[tuple[str, int, int]] = []
    for _ in range(count):
        (stored_name_len,) = struct.unpack_from("<I", pack_bytes, pos)
        pos += 4

        name_len = stored_name_len - 1
        name = pack_bytes[pos : pos + name_len].decode("utf-8")
        pos += name_len

        offset, size = struct.unpack_from("<II", pack_bytes, pos)
        pos += 8
        entries.append((name, offset, size))

    return pos, entries


def extract_encoded(pack_bytes: bytes, data_start: int, offset: int, size: int) -> bytes:
    return pack_bytes[data_start + offset : data_start + offset + size]


def decode_entry(encoded: bytes) -> str:
    return bytes(byte ^ 0xFF for byte in encoded).decode("utf-8", errors="ignore")


def encode_entry(text: str) -> bytes:
    return bytes(byte ^ 0xFF for byte in text.encode("utf-8"))


def rebuild_pack(
    pack_bytes: bytes,
    data_start: int,
    entries: list[tuple[str, int, int]],
    replacements: dict[str, bytes],
) -> bytes:
    header = bytearray()
    header.extend(struct.pack("<I", len(entries)))

    data_block = bytearray()
    rebuilt_entries: list[tuple[str, int, int]] = []
    for name, offset, size in entries:
        payload = replacements.get(name)
        if payload is None:
            payload = extract_encoded(pack_bytes, data_start, offset, size)

        rebuilt_entries.append((name, len(data_block), len(payload)))
        data_block.extend(payload)

    for name, offset, size in rebuilt_entries:
        name_bytes = name.encode("utf-8")
        header.extend(struct.pack("<I", len(name_bytes) + 1))
        header.extend(name_bytes)
        header.extend(struct.pack("<II", offset, size))

    return bytes(header + data_block)


def find_window_block(text: str, control_name: str) -> str:
    name_token = f'Name="{control_name}"'
    name_index = text.find(name_token)
    if name_index < 0:
        raise ValueError(f"missing control {control_name}")

    start = text.rfind("<Window", 0, name_index)
    if start < 0:
        raise ValueError(f"could not find start of {control_name}")

    end = text.find("</Window>", name_index)
    if end < 0:
        raise ValueError(f"could not find end of {control_name}")
    end += len("</Window>")

    while end < len(text) and text[end] in "\r\n":
        end += 1

    return text[start:end]


def replace_control_name(block: str, old_name: str, new_name: str) -> str:
    return block.replace(f'Name="{old_name}"', f'Name="{new_name}"', 1)


def patch_ui_text(text: str) -> tuple[str, int]:
    source_blocks = {
        "RPositionBtn": find_window_block(text, "RPositionBtn_0"),
        "BPositionBtn": find_window_block(text, "BPositionBtn_0"),
    }

    existing = {
        match.group(1)
        for match in re.finditer(r'Name="([RB]PositionBtn_-?\d+)"', text)
    }

    additions: list[str] = []
    for prefix, block in source_blocks.items():
        source_name = f"{prefix}_0"
        for value in ALIAS_RANGE:
            alias_name = f"{prefix}_{value}"
            if alias_name in existing:
                continue
            additions.append(replace_control_name(block, source_name, alias_name))
            existing.add(alias_name)

    if not additions:
        return text, 0

    insert_after = text.find(source_blocks["BPositionBtn"]) + len(source_blocks["BPositionBtn"])
    patched = text[:insert_after] + "".join(additions) + text[insert_after:]
    return patched, len(additions)


def patch_pak(path: Path, make_backup: bool) -> str:
    pack_bytes = path.read_bytes()
    data_start, entries = read_entries(pack_bytes)

    replacements: dict[str, bytes] = {}
    results: list[str] = []
    for name, offset, size in entries:
        if name not in TARGET_ENTRIES:
            continue
        text = decode_entry(extract_encoded(pack_bytes, data_start, offset, size))
        patched, count = patch_ui_text(text)
        if count:
            replacements[name] = encode_entry(patched)
        results.append(f"{name}:{count}")

    if not replacements:
        return "already patched (" + ", ".join(results) + ")"

    if make_backup:
        backup = path.with_name(path.name + BACKUP_SUFFIX)
        if not backup.exists():
            shutil.copy2(path, backup)

    path.write_bytes(rebuild_pack(pack_bytes, data_start, entries, replacements))
    return "patched " + ", ".join(results)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Add ready-room R/BPositionBtn aliases for all signed byte values to avoid null UI lookups."
    )
    parser.add_argument("pak", type=Path)
    parser.add_argument("--no-backup", action="store_true")
    args = parser.parse_args()

    print(f"{args.pak}: {patch_pak(args.pak, not args.no_backup)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
