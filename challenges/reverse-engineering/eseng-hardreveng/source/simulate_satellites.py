#!/usr/bin/env python3
"""
This python3 script will act as a wrapper for the server binary and acts as the
environment that the server interacts with. as such, this python3 script will
spawn the server process but also generate the satellites.

to interface with this python3 script, a shared-object file is used to control
the dataflow from and to the server. this shared-object should ensure binary
output as specified below.
- stdout: regular program stdout
- stderr: treated as commands to update satellite data
- stdin: a little trickier, this will act as an input for the user and for
         returning the results of different commands
"""

import random
import subprocess as subp
import sys

# simulation details
NO_SATELLITES = 4
MAX_ANGLE = 360.0
GRID_SIZE = 12

class Satellite:
    _name_base = "Satellite "

    def __init__(self):
        self.theta = [-1,-1,-1] # -1 indicates un-initialized
        self.pos = (-1,-1,-1)
        self.name = ""
        self.symbol = ''

    def _orient_angle(self, tx, ty, tz):
        """
        orients the angle of the Satellite to the tuple `theta = (tx, ty, tz)`.

        for any angular component x, y or z in theta = (x, y, z), the component
        must be within the range of [0, 360) in degrees. this is a cyclical
        range
        """
        self.theta[0] = tx % MAX_ANGLE
        self.theta[1] = ty % MAX_ANGLE
        self.theta[2] = tz % MAX_ANGLE

    def add_angle(self, dtx, dty, dtz):
        """
        adds the specified angle to the Satellite, changing its orientation.
        this will call self._orient_angle
        """
        self._orient_angle(self.theta[0]+dtx, self.theta[1]+dty, self.theta[2]+dtz)

    def set_satellite(self, pos, theta, symbol):
        """
        sets the details of the satellite

        this should be used when generating satellite information
        """
        self.theta = [t%MAX_ANGLE for t in theta]
        self.pos = tuple([p%GRID_SIZE for p in pos])
        self.name = self._name_base + symbol
        self.symbol = symbol

class Simulation:
    def __init__(self):
        self.satellites = {}

        x_history = []
        y_history = []
        z_history = []

        # gen satellites
        for i in range(NO_SATELLITES):
            s = Satellite()

            new_pos = [-1,-1,-1]

            unique = False
            while(not unique):
                new_pos[0] = random.randint(0,GRID_SIZE)
                unique = new_pos[0] not in x_history

            unique = False
            while(not unique):
                new_pos[1] = random.randint(0,GRID_SIZE)
                unique = new_pos[1] not in y_history

            unique = False
            while(not unique):
                new_pos[2] = random.randint(0,GRID_SIZE)
                unique = new_pos[2] not in z_history

            new_theta = [random.random() * MAX_ANGLE for _ in range(3)]

            new_symbol = chr(ord('A') + i)

            s.set_satellite(tuple(new_pos), new_theta, new_symbol)

            self.satellites[new_symbol] = s

    def get_satellite(self, symbol):
        s = self.satellites[symbol]
        sys.stderr.write(
        f"""
{s.name} ({s.symbol})
pos: {s.pos}
theta: {s.theta}\n""")

if __name__ == "__main__":
    sim = Simulation()
    sim.get_satellite('A')
    sim.get_satellite('B')
    sim.get_satellite('C')
    sim.get_satellite('D')
