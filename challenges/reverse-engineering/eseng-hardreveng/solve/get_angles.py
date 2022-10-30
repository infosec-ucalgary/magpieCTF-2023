#!/usr/bin/env python3

import math
import numpy as np
import struct

def main():
    sat1_pos_str = input("Satellite A position: ")
    sat1_angle_str = input("Satellite A angle: ")

    sat1 = ([int(x) for x in sat1_pos_str.split(',')], 
        [float(x) for x in sat1_angle_str.split(',')])
    print(sat1)

    sat2_pos_str = input("Satellite B position: ")
    sat2_angle_str = input("Satellite B angle: ")

    sat2 = ([int(x) for x in sat2_pos_str.split(',')], 
        [float(x) for x in sat2_angle_str.split(',')])
    print(sat2)

    dx = math.atan(
            math.sqrt(pow(sat2[0][2] - sat1[0][2], 2)
                      +
                      pow(sat2[0][1] - sat1[0][1], 2))
            /
            (sat2[0][0] - sat1[0][0])
         ) - sat1[1][0] + 360

    dy = math.atan(
            math.sqrt(pow(sat2[0][2] - sat1[0][2], 2)
                      +
                      pow(sat2[0][0] - sat1[0][0], 2))
            /
            (sat2[0][1] - sat1[0][1])
         ) - sat1[1][1] + 360

    dz = math.atan(
            math.sqrt(pow(sat2[0][0] - sat1[0][0], 2)
                      +
                      pow(sat2[0][1] - sat1[0][1], 2))
            /
            (sat2[0][2] - sat1[0][2])
         ) - sat1[1][2] + 360

    dx_hex = hex(int.from_bytes(np.array([dx], '>f2').tobytes(), "big"))
    dy_hex = hex(int.from_bytes(np.array([dy], '>f2').tobytes(), "big"))
    dz_hex = hex(int.from_bytes(np.array([dz], '>f2').tobytes(), "big"))

    print(f"""
lui     $zp $dx 0x{dx_hex[2:4]}
ori     $dx $dx 0x{dx_hex[4:6]}
lui     $zp $dy 0x{dy_hex[2:4]}
ori     $dy $dy 0x{dy_hex[4:6]}
lui     $zp $dz 0x{dz_hex[2:4]}
ori     $dz $dz 0x{dz_hex[4:6]}
    """)
    return

if __name__ == "__main__":
    main()
