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
import subprocessing as subp

# simulation details
NO_SATELLITES = 4
MAX_ANGLE = 360.0
GRID_SIZE = 12

class Satellite:
    _name_base = "Satellite "

    def __init__(self):
        self.theta = (-1,-1,-1) # -1 indicates un-initialized
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

    def set_satellite(self, theta, pos, symbol):
        """
        sets the details of the satellite

        this should be used when generating satellite information
        """
        self.theta = tuple([t%MAX_ANGLE for t in theta])
        self.pos = tuple([p%GRID_SIZE for p in pos])
        self.name = self._name_base += symbol
        self.symbol = symbol
