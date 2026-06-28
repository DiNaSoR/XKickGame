from __future__ import annotations

import argparse
import shutil
import struct
from pathlib import Path


DEFAULT_SOURCE = "Process_N_ReadyRoom_New.ui"
DEFAULT_TARGET = "Process_W_ReadyRoom.ui"
BACKUP_SUFFIX = ".orig-readyroom-ui"


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


def patch_readyroom_ui(path: Path, source_name: str, target_name: str, make_backup: bool) -> str:
    pack_bytes = path.read_bytes()
    data_start, entries = read_entries(pack_bytes)
    by_name = {name: (offset, size) for name, offset, size in entries}

    if source_name not in by_name:
        raise ValueError(f"{path}: missing source entry {source_name}")
    if target_name not in by_name:
        raise ValueError(f"{path}: missing target entry {target_name}")

    source_offset, source_size = by_name[source_name]
    target_offset, target_size = by_name[target_name]
    source_payload = extract_encoded(pack_bytes, data_start, source_offset, source_size)
    target_payload = extract_encoded(pack_bytes, data_start, target_offset, target_size)

    required_controls = ("ReadyRoomMain", "RPlayerPosition_H_0", "SetPositionBtn4", "RPositionBtn_0")
    source_text = decode_entry(source_payload)
    missing_from_source = [name for name in required_controls if name not in source_text]
    if missing_from_source:
        raise ValueError(f"{source_name} is missing expected controls: {', '.join(missing_from_source)}")

    target_text = decode_entry(target_payload)
    if all(name in target_text for name in required_controls):
        return "already patched"

    rebuilt = rebuild_pack(pack_bytes, data_start, entries, {target_name: source_payload})
    if make_backup:
        backup = path.with_name(path.name + BACKUP_SUFFIX)
        if not backup.exists():
            shutil.copy2(path, backup)

    path.write_bytes(rebuilt)
    return f"patched {target_name} from {source_name}"


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Patch the installed client Ui.pak so wide ready-room loads the working ready-room UI."
    )
    parser.add_argument("pak", type=Path)
    parser.add_argument("--source", default=DEFAULT_SOURCE)
    parser.add_argument("--target", default=DEFAULT_TARGET)
    parser.add_argument("--no-backup", action="store_true")
    args = parser.parse_args()

    result = patch_readyroom_ui(args.pak, args.source, args.target, not args.no_backup)
    print(f"{args.pak}: {result}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
