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

NO_SATELLITES = 4
MAX_ANGLE = 360.0

class Satellite:
    def __init__(self):
        self.theta = (-1,-1,-1)
        self.pos = (-1,-1,-1)
        self.name = ""
        self.symbol = ''
