from __future__ import annotations

import argparse
import socket
import struct
import threading
from datetime import datetime
from pathlib import Path


HEAD_SIZE = 12
CM_GAME_LOGIN = 2000
CM_GAME_EXIT = 2001
CM_CREATE_ROOM = 2203
CM_ROOM_INFO = 2200

SECURE_ARRAY = (
    b"CarpediemSeizeTheDayBoysMakeYourLivesExtraodinaryWOrkHard!"
    b"HaveFun!MakeHistory!BeTheChangeYouWannaSeeInTheWorldBy Mutter 2009.07@!#"
)


class DirectionCrypto:
    def __init__(self) -> None:
        self.seq = 0
        self.enabled = False

    def init(self, seed: int) -> None:
        self.seq = seed
        self.enabled = True

    def next_key(self) -> int:
        self.seq += 1
        if self.seq > 65535:
            self.seq = 0
        n_array1 = self.seq % 128
        n_array2 = (self.seq // 127) % 128
        return self.seq ^ (SECURE_ARRAY[n_array1] * SECURE_ARRAY[n_array2])

    def decrypt(self, body: bytes, packet_seq: int) -> bytes:
        expected = self.next_key()
        if packet_seq != expected:
            # Still decrypt with the current rolling sequence so logs show useful bytes.
            pass

        n_array1 = self.seq % 128
        n_array2 = (self.seq // 127) % 128
        if n_array1 == n_array2:
            n_array2 += 1

        out = bytearray(body)
        for index, value in enumerate(out):
            out[index] = value ^ SECURE_ARRAY[n_array2] ^ SECURE_ARRAY[n_array1]
            n_array1 += 1
            if n_array1 > 127:
                n_array1 = 0
            n_array2 += 1
            if n_array2 > 127:
                n_array2 = 0
        return bytes(out)


class Logger:
    def __init__(self, path: Path) -> None:
        self.path = path
        self.lock = threading.Lock()
        path.parent.mkdir(parents=True, exist_ok=True)

    def write(self, message: str) -> None:
        line = f"{datetime.now().isoformat(timespec='seconds')} {message}"
        with self.lock:
            with self.path.open("a", encoding="utf-8") as handle:
                handle.write(line + "\n")
        print(line, flush=True)


def recvall(sock: socket.socket, size: int) -> bytes | None:
    chunks = bytearray()
    while len(chunks) < size:
        data = sock.recv(size - len(chunks))
        if not data:
            return None
        chunks.extend(data)
    return bytes(chunks)


def decode_room_info(body: bytes) -> str:
    if len(body) < 0x1E8 or body[0] != 0:
        return f"room_info response={body[0] if body else 'empty'} body_len={len(body)}"

    room = body[1:]
    # Runtime CSCRoomInfo uses a 0x5b-byte CRoomInfo prefix before CReserveSeat.
    home = list(room[0x5B : 0x5B + 6])
    away = list(room[0x5B + 0x84 : 0x5B + 0x84 + 6])
    fields = {
        "state": room[0],
        "mode": room[1],
        "ground": room[0x48],
        "ball": room[0x49],
        "attack": room[0x4C],
        "scale": room[0x4D],
        "ai": room[0x4E],
        "point": struct.unpack_from("<h", room, 0x4F)[0],
        "start": room[0x51],
        "end": room[0x52],
        "max": room[0x54],
        "checks": list(room[0x55 : 0x5B]),
    }
    valid = {0, 1, 10, 11, 12, 13, 14, 20, 21, 22, 23, 24, 30, 31, 32, 33, 40}
    invalid = [value for value in home + away if value not in valid]
    return (
        f"room_info fields={fields} "
        f"home={home} away={away} "
        f"home_names={[f'RPositionBtn_{v}' for v in home]} "
        f"away_names={[f'BPositionBtn_{v}' for v in away]} "
        f"invalid_positions={invalid}"
    )


def decode_create_room(body: bytes) -> str:
    if len(body) < 79:
        return f"create_room body_len={len(body)}"
    home = list(body[0x43:0x49])
    away = list(body[0x49:0x4F])
    fields = {
        "attack": body[0x36],
        "scale": body[0x37],
        "ai": body[0x38],
        "point": struct.unpack_from("<h", body, 0x39)[0],
        "start": body[0x3B],
        "end": body[0x3C],
        "checks": list(body[0x3D:0x42]),
        "max": body[0x42],
    }
    return f"create_room fields={fields} home={home} away={away}"


def pipe(
    source: socket.socket,
    target: socket.socket,
    direction: str,
    crypto: DirectionCrypto,
    logger: Logger,
    on_game_login_seed,
) -> None:
    try:
        while True:
            header = recvall(source, HEAD_SIZE)
            if header is None:
                return
            command, body_size, sequence = struct.unpack("<iii", header)
            body = recvall(source, body_size)
            if body is None:
                return

            decoded = body
            encrypted = crypto.enabled and command > CM_GAME_EXIT
            if encrypted:
                decoded = crypto.decrypt(body, sequence)

            logger.write(
                f"{direction} cmd={command} size={body_size} seq={sequence} encrypted={encrypted}"
            )
            if direction == "C>S" and command == CM_CREATE_ROOM:
                logger.write(f"{direction} {decode_create_room(decoded)}")
            if direction == "S>C" and command == CM_ROOM_INFO:
                logger.write(f"{direction} {decode_room_info(decoded)}")

            target.sendall(header + body)

            if direction == "S>C" and command == CM_GAME_LOGIN:
                on_game_login_seed(sequence)
    except OSError as exc:
        logger.write(f"{direction} closed: {exc}")


def serve(listen_host: str, listen_port: int, target_host: str, target_port: int, logger: Logger) -> None:
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((listen_host, listen_port))
    server.listen(5)
    logger.write(f"listening {listen_host}:{listen_port} -> {target_host}:{target_port}")

    while True:
        client, client_addr = server.accept()
        upstream = socket.create_connection((target_host, target_port))
        logger.write(f"accepted {client_addr[0]}:{client_addr[1]}")

        client_to_server = DirectionCrypto()
        server_to_client = DirectionCrypto()

        def init_both(seed: int) -> None:
            logger.write(f"secure seed={seed}")
            client_to_server.init(seed)
            server_to_client.init(seed)

        threading.Thread(
            target=pipe,
            args=(client, upstream, "C>S", client_to_server, logger, init_both),
            daemon=True,
        ).start()
        threading.Thread(
            target=pipe,
            args=(upstream, client, "S>C", server_to_client, logger, init_both),
            daemon=True,
        ).start()


def parse_endpoint(value: str) -> tuple[str, int]:
    host, port = value.rsplit(":", 1)
    return host, int(port)


def main() -> int:
    parser = argparse.ArgumentParser(description="Log decoded XKick game TCP packets via a local proxy.")
    parser.add_argument("--listen", default="127.0.0.1:3002")
    parser.add_argument("--target", default="127.0.0.1:3001")
    parser.add_argument("--log", type=Path, default=Path("server/runtime/logs/game_tcp_debug_proxy.log"))
    args = parser.parse_args()

    listen_host, listen_port = parse_endpoint(args.listen)
    target_host, target_port = parse_endpoint(args.target)
    serve(listen_host, listen_port, target_host, target_port, Logger(args.log))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
