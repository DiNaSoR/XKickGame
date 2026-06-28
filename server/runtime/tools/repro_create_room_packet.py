from __future__ import annotations

import argparse
import socket
import struct
from dataclasses import dataclass


HEAD_SIZE = 12

CM_GAME_LOGIN = 2000
CM_GAME_EXIT = 2001
CM_UDP_CONFIRM = 2002
CM_SCHEDULE_LIST = 2004
CM_PLAYER_INFO = 2100
CM_ROOM_INFO = 2200
CM_CREATE_ROOM = 2203
CM_CHOICE_ROOM = 2205

CM_CHANGE_JANG = 2302
CM_ATHLETE_INFO = 2303
CM_ATHLETE_END = 2304

SECURE_ARRAY = (
    b"CarpediemSeizeTheDayBoysMakeYourLivesExtraodinaryWOrkHard!"
    b"HaveFun!MakeHistory!BeTheChangeYouWannaSeeInTheWorldBy Mutter 2009.07@!#"
)


@dataclass
class Packet:
    command: int
    sequence: int
    body: bytes


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

    def crypt_body(self, body: bytes) -> tuple[int, bytes]:
        packet_seq = self.next_key()
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
        return packet_seq, bytes(out)

    def decrypt(self, body: bytes, packet_seq: int) -> bytes:
        expected_seq, decoded = self.crypt_body(body)
        if packet_seq != expected_seq:
            print(f"warning: packet sequence mismatch got={packet_seq} expected={expected_seq}")
        return decoded


def recvall(sock: socket.socket, size: int) -> bytes | None:
    chunks = bytearray()
    while len(chunks) < size:
        data = sock.recv(size - len(chunks))
        if not data:
            return None
        chunks.extend(data)
    return bytes(chunks)


def recv_packet(sock: socket.socket, crypto: DirectionCrypto) -> Packet | None:
    header = recvall(sock, HEAD_SIZE)
    if header is None:
        return None
    command, body_size, sequence = struct.unpack("<iii", header)
    body = recvall(sock, body_size)
    if body is None:
        return None
    if crypto.enabled and command > CM_GAME_EXIT:
        body = crypto.decrypt(body, sequence)
    return Packet(command, sequence, body)


def send_packet(sock: socket.socket, crypto: DirectionCrypto, command: int, body: bytes) -> None:
    sequence = -1
    encoded = body
    if crypto.enabled and command > CM_GAME_EXIT:
        sequence, encoded = crypto.crypt_body(body)
    header = struct.pack("<iii", command, len(body), sequence)
    sock.sendall(header + encoded)


def fixed_text(value: str, size: int) -> bytes:
    data = value.encode("cp949", errors="replace")[:size]
    return data + (b"\x00" * (size - len(data)))


def create_room_body() -> bytes:
    title = fixed_text("test", 47)
    password = fixed_text("", 5)
    home = bytes([10, 20, 30, 1, 0, 40])
    away = bytes([10, 20, 30, 1, 0, 40])
    return struct.pack(
        "<bb47s5sbbbHbbbbbbb6s6s",
        1,  # ROOM_STATE_NORMAL
        0,  # ROOM_MODE_NORMAL
        title,
        password,
        1,  # ROOM_ATTACK_RANDOM
        5,  # ROOM_SCALE_FIVE
        1,  # ROOM_AI_ALL
        0,  # point
        1,  # start level
        60,  # end level
        0,  # club
        1,  # time
        1,  # weather
        0,  # view
        0,  # view chat
        10,  # max count
        home,
        away,
    )


def describe_room_info(body: bytes) -> None:
    print(f"room_info response={body[0] if body else 'empty'} body_len={len(body)}")
    if len(body) < 488 or body[0] != 0:
        return

    room = body[1:]
    valid = {0, 1, 10, 11, 12, 13, 14, 20, 21, 22, 23, 24, 30, 31, 32, 33, 40}
    for seat_offset in (0x5A, 0x5B):
        home = list(room[seat_offset : seat_offset + 6])
        away = list(room[seat_offset + 0x84 : seat_offset + 0x84 + 6])
        invalid = [value for value in home + away if value not in valid]
        print(
            f"seat_offset={seat_offset:#x} "
            f"home={home} away={away} invalid={invalid} "
            f"home_names={[f'RPositionBtn_{value}' for value in home]} "
            f"away_names={[f'BPositionBtn_{value}' for value in away]}"
        )

    fields = {
        "state": room[0],
        "mode": room[1],
        "course": room[2],
        "room_seq": struct.unpack_from("<i", room, 3)[0],
        "attack": room[0x4C],
        "scale": room[0x4D],
        "ai": room[0x4E],
        "point": struct.unpack_from("<h", room, 0x4F)[0],
        "start": room[0x51],
        "end": room[0x52],
        "attack_team": room[0x53],
        "max": room[0x54],
        "checks": list(room[0x55:0x5B]),
    }
    print(f"fields={fields}")


def drain(sock: socket.socket, crypto: DirectionCrypto, label: str) -> list[Packet]:
    packets: list[Packet] = []
    while True:
        try:
            packet = recv_packet(sock, crypto)
        except socket.timeout:
            break
        if packet is None:
            break
        packets.append(packet)
        print(f"{label} recv cmd={packet.command} size={len(packet.body)} seq={packet.sequence}")
        if packet.command == CM_ROOM_INFO:
            describe_room_info(packet.body)
    return packets


def run(host: str, port: int, member_seq: int, player_seq: int) -> int:
    client_crypto = DirectionCrypto()
    server_crypto = DirectionCrypto()

    with socket.create_connection((host, port), timeout=5) as sock:
        sock.settimeout(2)

        login_body = struct.pack("<iiii", member_seq, player_seq, 0, 0)
        send_packet(sock, client_crypto, CM_GAME_LOGIN, login_body)
        login = recv_packet(sock, server_crypto)
        if login is None:
            print("login failed: no response")
            return 1
        print(f"login recv cmd={login.command} size={len(login.body)} seq={login.sequence} body={list(login.body)}")
        if login.command != CM_GAME_LOGIN or not login.body or login.body[0] != 0:
            return 1

        client_crypto.init(login.sequence)
        server_crypto.init(login.sequence)

        send_packet(sock, client_crypto, CM_SCHEDULE_LIST, struct.pack("<i", 101))
        drain(sock, server_crypto, "schedule")

        send_packet(sock, client_crypto, CM_UDP_CONFIRM, struct.pack("<i", player_seq))
        drain(sock, server_crypto, "udp")

        send_packet(sock, client_crypto, CM_PLAYER_INFO, struct.pack("<i", player_seq))
        drain(sock, server_crypto, "player_info")

        send_packet(sock, client_crypto, CM_CREATE_ROOM, create_room_body())
        packets = drain(sock, server_crypto, "create_room")

        room_seq = 0
        for packet in packets:
            if packet.command == CM_CHOICE_ROOM and len(packet.body) >= 6 and packet.body[0] == 0:
                room_seq = struct.unpack_from("<i", packet.body, 1)[0]
                print(f"choice_room room_seq={room_seq} team={packet.body[5]}")

        send_packet(sock, client_crypto, CM_ROOM_INFO, struct.pack("<ih", player_seq, room_seq))
        drain(sock, server_crypto, "room_info")

        send_packet(sock, client_crypto, CM_GAME_EXIT, b"\x00")

    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="Create a room through the game TCP protocol and decode room info.")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=3001)
    parser.add_argument("--member-seq", type=int, default=1)
    parser.add_argument("--player-seq", type=int, default=4)
    args = parser.parse_args()
    return run(args.host, args.port, args.member_seq, args.player_seq)


if __name__ == "__main__":
    raise SystemExit(main())
