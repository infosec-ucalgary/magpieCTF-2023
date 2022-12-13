#!/usr/bin/env python3
import sys

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

def parse_line_instruction(line: bytes) -> bytes:
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

    return int.to_bytes(instr_int, 2, "big")

def main():
    lines = sys.stdin.buffer.readlines()
    for line in lines:
        sys.stderr.buffer.write(b"| " + line)
        sys.stdout.buffer.write(parse_line_instruction(line))
    sys.stdout.buffer.write(b'\n')
    return

if __name__ == "__main__":
    main()
