#!/usr/bin/env python3

import math
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

    print("dx", dx, struct.pack(">f", dx))
    print("dy", dy, struct.pack(">f", dy))
    print("dz", dz, struct.pack(">f", dz))
    return

if __name__ == "__main__":
    main()
