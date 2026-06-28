from __future__ import annotations

import argparse
import shutil
import struct
from pathlib import Path


PATCHES = {
    # Server CreateRoom expects one extra byte before the client room options.
    # Shift post-level option reads back to the installed 79-byte client layout.
    0x809CE20: (bytes.fromhex("8a 40 48 88 42 5d"), bytes.fromhex("8a 40 47 88 42 5d")),
    0x809CE2C: (bytes.fromhex("8a 40 49 88 42 5e"), bytes.fromhex("8a 40 48 88 42 5e")),
    0x809CE38: (bytes.fromhex("8a 40 4a 88 42 61"), bytes.fromhex("8a 40 49 88 42 61")),
    0x809CE44: (bytes.fromhex("8a 40 4b 88 42 62"), bytes.fromhex("8a 40 4a 88 42 62")),
    0x809CE6A: (bytes.fromhex("8a 40 4c 88 42 63"), bytes.fromhex("8a 40 4b 88 42 63")),
    0x809CE76: (bytes.fromhex("8a 40 4d 88 42 64"), bytes.fromhex("8a 40 4c 88 42 64")),
    0x809CE82: (bytes.fromhex("8a 40 4e 88 42 65"), bytes.fromhex("8a 40 4d 88 42 65")),
    0x809CE8E: (bytes.fromhex("8a 40 4f 88 42 60"), bytes.fromhex("8a 40 4e 88 42 60")),
    0x809CEAA: (bytes.fromhex("83 c0 50 50"), bytes.fromhex("83 c0 4f 50")),
    0x809CEC5: (bytes.fromhex("83 c0 56 50"), bytes.fromhex("83 c0 55 50")),
}


def va_to_offset(data: bytes, va: int) -> int:
    if data[:4] != b"\x7fELF" or data[4] != 1:
        raise ValueError("expected ELF32 binary")

    endian = "<" if data[5] == 1 else ">"
    header = struct.unpack_from(endian + "HHIIIIIHHHHHH", data, 16)
    e_phoff = header[4]
    e_phentsize = header[8]
    e_phnum = header[9]

    for index in range(e_phnum):
        off = e_phoff + index * e_phentsize
        p_type, p_offset, p_vaddr, _p_paddr, p_filesz, _p_memsz, _p_flags, _p_align = struct.unpack_from(
            endian + "IIIIIIII", data, off
        )
        if p_type == 1 and p_vaddr <= va < p_vaddr + p_filesz:
            return p_offset + (va - p_vaddr)

    raise ValueError(f"VA {va:#x} is not in a loadable file segment")


def patch_file(path: Path, make_backup: bool) -> str:
    data = bytearray(path.read_bytes())
    changed = False

    for va, (old, new) in PATCHES.items():
        offset = va_to_offset(data, va)
        current = bytes(data[offset : offset + len(old)])
        if current == new:
            continue
        if current != old:
            raise ValueError(
                f"{path}: unexpected bytes at {va:#x} file offset {offset:#x}: "
                f"{current.hex(' ')}"
            )
        data[offset : offset + len(new)] = new
        changed = True

    if not changed:
        return "already patched"

    if make_backup:
        backup = path.with_name(path.name + ".orig-create-room-layout")
        if not backup.exists():
            shutil.copy2(path, backup)

    path.write_bytes(data)
    return "patched"


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Patch legacy game server CreateRoom field offsets for the installed 79-byte client packet."
    )
    parser.add_argument("targets", nargs="+", type=Path)
    parser.add_argument("--no-backup", action="store_true")
    args = parser.parse_args()

    for target in args.targets:
        print(f"{target}: {patch_file(target, not args.no_backup)}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
