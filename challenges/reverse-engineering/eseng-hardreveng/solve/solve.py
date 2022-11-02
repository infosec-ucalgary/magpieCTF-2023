#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template --host localhost --port 31185 ../source/build/shift_satellite
from pwn import *

# Set up pwntools for the correct architecture
exe = context.binary = ELF('../source/build/shift_satellite')

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or 'localhost'
port = int(args.PORT or 31185)

def start_local(argv=[], *a, **kw):
    '''Execute the target binary locally'''
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process([exe.path] + argv, *a, **kw)

def start_remote(argv=[], *a, **kw):
    '''Connect to the process on the remote host'''
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io

def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.LOCAL:
        return start_local(argv, *a, **kw)
    else:
        return start_remote(argv, *a, **kw)

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
tbreak main
continue
'''.format(**locals())

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      PIE enabled

import sys
import math

class Satellite:
    ARG_MAP = {
        b"addi" : 0b00,
        b"ori"  : 0b01,
        b"slli" : 0b10,
        b"lui"  : 0b11,
    }

    REG_MAP = {
        b"$zp": 0b000,
        b"$ip": 0b001,
        b"$rp": 0b010,
        b"$fp": 0b011,
        b"$sp": 0b100,
        b"$dx": 0b101,
        b"$dy": 0b110,
        b"$dz": 0b111,
    }

    def __init__(self, symbol:chr):
        self.theta = {
            "x": 0,
            "y": 0,
            "z": 0,
        }
        self.position = {
            "x": 0,
            "y": 0,
            "z": 0,
        }
        self.symbol = symbol

    def point_to_next_sat(self, next_sat: object) -> bytes:
        dx = math.atan(
                math.sqrt(pow(next_sat.position['z'] - self.position['z'], 2)
                          +
                          pow(next_sat.position['y'] - self.position['y'], 2))
                /
                (next_sat.position['x'] - self.position['x'])
             ) * 180 / math.pi - self.theta['x'] + 360

        dy = math.atan(
                math.sqrt(pow(next_sat.position['x'] - self.position['y'], 2)
                          +
                          pow(next_sat.position['z'] - self.position['z'], 2))
                /
                (next_sat.position['y'] - self.position['y'])
             ) * 180 / math.pi - self.theta['y'] + 360
        dz = math.atan(
                math.sqrt(pow(next_sat.position['x'] - self.position['y'], 2)
                          +
                          pow(next_sat.position['y'] - self.position['y'], 2))
                /
                (next_sat.position['z'] - self.position['z'])
             ) * 180 / math.pi - self.theta['z'] + 360

        sys.stderr.write(f"desired changes: {dx}, {dy}. {dz}")

        dx_hex = hex(int.from_bytes(np.array([dx], '>f2').tobytes(), "big"))
        dy_hex = hex(int.from_bytes(np.array([dy], '>f2').tobytes(), "big"))
        dz_hex = hex(int.from_bytes(np.array([dz], '>f2').tobytes(), "big"))

        return f"""
lui     $zp $dx 0x{dx_hex[2:4]}
ori     $dx $dx 0x{dx_hex[4:6]}
lui     $zp $dy 0x{dy_hex[2:4]}
ori     $dy $dy 0x{dy_hex[4:6]}
lui     $zp $dz 0x{dz_hex[2:4]}
ori     $dz $dz 0x{dz_hex[4:6]}
    """

    def assemble_asm(instructions:bytes) -> bytes:
        machine_code = b""

        for line in instructions.split(b'\n'):
            # remove comments (after ';')
            line = line.split(b';')[0]
            # split across whitespaces
            instr_parts = line.split()

            instr_int = 0
            # set the argument
            instr_int = ARG_MAP[instr_parts[0]]
            # set the register values
            instr_int = instr_int << 3 | REG_MAP[instr_parts[1]]
            instr_int = instr_int << 3 | REG_MAP[instr_parts[2]]

            immediate = 0
            try:
                if(instr_parts[3][:2] == b"0x"):
                    immediate = int(instr_parts[3][2:], 16)
                elif(instr_parts[3][:2] == b"0b"):
                    immediate = int(instr_parts[3][2:], 2)
                else:
                    immediate = int(instr_parts[3])
            except Exception as e:
                print("immediate value is not valid :(")
                print(e)
                exit(-1)

            instr_int = instr_int << 8 | immediate

            machine_code.append(int.to_bytes(instr_int, 2, "big"))

        return machine_code

    def __str__(self):
        return f"""sat: {self.symbol}
pos: {self.position['x']}, {self.position['y']}, {self.position['z']}
angle: {self.theta['x']}, {self.theta['y']}, {self.theta['z']}"""

def parse_grid(satellites:dict, grid:bytes) -> None:
    # this grid should start with the label of the vertical axis
    vertical_axis_label = chr(grid[0])
    horizontal_axis_label = chr(grid[-5])

    grid = b'\n'.join(grid.split(b'\n')[1:-2][::-1])

    # replace unnecessary characters, placing symbolsto the right of their
    # index.
    grid = grid.replace(b'|', b'')
    grid = grid.replace(b' ', b'')
    grid = grid.replace(b']', b'')

    vert_val = 0
    hori_val = 0

    # sys.stderr.buffer.write(grid + b'\n')
    for line in grid.split(b'\n'):
        for char in line:
            spot = chr(char)
            if spot == '[':
                hori_val += 1
                continue
            # the spot is actually the symbol
            if spot not in satellites.keys():
                satellites[spot] = Satellite(spot)

            satellites[spot].position[horizontal_axis_label] = hori_val - 1
            satellites[spot].position[vertical_axis_label] = vert_val
        vert_val += 1
        hori_val = 0

    for sat in satellites.values():
        sys.stderr.write(f"{sat}\n")

    return

def solve() -> bool:
    io = start()

    # shellcode = asm(shellcraft.sh())
    # payload = fit({
    #     32: 0xdeadbeef,
    #     'iaaa': [1, 2, 'Hello', 3]
    # }, length=128)
    # io.send(payload)
    # flag = io.recv(...)
    # log.success(flag)

    satellites = {}

    # get positions and orientations of each satellite
    io.recvuntil(b"awaiting input...\n") # drop splash screen :(

    io.send(b"POSN\n")
    io.recvuntil(b'| ')

    grid = io.recvuntil(b'+')
    sys.stderr.buffer.write(grid + b'\n')
    parse_grid(satellites, grid) # x, y

    io.recvuntil(b"| ")
    grid = io.recvuntil(b'+')
    sys.stderr.buffer.write(grid + b'\n')
    parse_grid(satellites, grid) # z, y

    # between x and z, there should be no change
    io.recvuntil(b"| ")
    grid = io.recvuntil(b'\na')
    sys.stderr.buffer.write(grid + b'\n')
    parse_grid(satellites, grid) # x, z

    return False

if __name__ == "__main__":
    print(solve())
