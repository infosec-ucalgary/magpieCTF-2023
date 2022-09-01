/**
 * gen_satellites.c
 * developed by e-seng for magpieCTF 2023
 *
 * this binary will be connected to by the server backend and will return the
 * raw data of satellites. effectively, all satellite data will be stored
 * locally within this running binary, so that the shift_satellite binary will
 * appear like it sends signals to the satellites directly instead of spawning
 * the satellites for the sake of the challenge.
 *
 * creating this binary will also reduce the complexity of the server binary,
 * but will increase the complexity of developing this challenge ;-;. using
 * these two running binaries and a common shared binary, the server binary will
 * seem like it is making calls to some futuristic communication device while
 * encapsulating the underlying process dictating how this challenge works.
 *
 * tl:dr, i'm doing this for added immersiveness, which is pain ig.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
