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
#include "./connection_helper.h"

#define NO_SATELLITES 4
#define GRID_EDGE_SIZE 12
#define MAX_ANGLE 360.0

/**
 * Generate the satellites that will be placed throughout space at varying
 * locations and orientations.
 *
 * Each of the NO_SATELLITES need to be placed in different sections of the
 * space. considering a cube with the transcever and receiver at two opposite
 * sides across the cube. each of the satellites need to be placed in each third
 * but at one of the 9 sections within that slice.
 *
 * eg. if this is a 3-angled review of a cube, then satellites A, B, C could be
 * oriented like this below. 'R' is the receiver, 'S' is the sender.
 *
 *    x > [O]         z > [O]         z >
 *    [ ] [ ] [C] |   [C] [ ] [ ] |   [C] [ ] [ ]
 *  ^ [A] [ ] [ ] | ^ [ ] [A] [ ] | ^ [ ] [ ] [B]
 *  y [ ] [B] [ ] | y [ ] [ ] [B] | x [ ] [A] [ ]
 *        [S]             [S]
 * this represents:
 * N(x,y,z)
 * A(0,1,1)
 * B(1,0,2)
 * C(2,2,0)
 *
 * note: in the actual grid, it would be okay for two of the satellites to
 * be in the same section (for the y/z slice at least) as long as the satellites
 * do not overlap.
 */
Satellite ** generate_satellite_info();

int main(int argc, char ** argv){
  char command[5]; // max command length is 4
  fprintf(stderr, "connected\n");

  while(1){
    memset(command, 0, 5);
    scanf("%4s", command);
    fprintf(stderr, "%s\n", command);
    // if(strncmp(command, "init", 4) == 0) fprintf(stderr, "initialize satellites\n");
  }
}

Satellite ** generate_satellite_info(){
  // seed the random value
  srand(time(NULL));

  const char * base_name = "Satellite _"; // replace _ with an identifier
  const int REPLACE_INDEX = 10;

  const int PART_SIZE = GRID_EDGE_SIZE / NO_SATELLITES;
  int x_history[NO_SATELLITES]; // keep track of previous satellite x positions
  int z_history[NO_SATELLITES]; // keep track of previous satellite z positions

  Satellite ** satellites = malloc(sizeof(Satellite *) * NO_SATELLITES);
  for(int i = 0; i < NO_SATELLITES; i++){
    satellites[i] = malloc(sizeof(Satellite));

    // generate the necessary coordinates and angles
    satellites[i] -> pos_y = rand() % PART_SIZE + i * PART_SIZE; // ensure spacing across x

    int suggested_x, suggested_z;

    short unique = 1;
    do{
      unique = 1;
      suggested_x =  rand() % GRID_EDGE_SIZE;

      for(int j = i; j > 0; j--){
        unique &= suggested_x != x_history[j-1];
      }
    }while(!unique);
    satellites[i] -> pos_x = suggested_x;
    x_history[i] = suggested_x;

    do{
      unique = 1;
      suggested_z =  rand() % GRID_EDGE_SIZE;

      for(int j = i; j > 0; j--){
        unique &= suggested_z != z_history[j-1];
      }
    }while(!unique);
    satellites[i] -> pos_z = suggested_z;
    z_history[i] = suggested_z;

    satellites[i] -> theta_x = rand() / (float) RAND_MAX * MAX_ANGLE;
    satellites[i] -> theta_y = rand() / (float) RAND_MAX * MAX_ANGLE;
    satellites[i] -> theta_z = rand() / (float) RAND_MAX * MAX_ANGLE;

    // set name and symbol
    satellites[i] -> symbol = 'A' + i;
    satellites[i] -> name = malloc(strlen(base_name) * sizeof(char));
    strcpy(satellites[i] -> name, base_name);
    satellites[i] -> name[REPLACE_INDEX] = satellites[i] -> symbol;

    // debugging
    fprintf(stderr, "Satellite %c: pos:(%d, %d, %d), angle:(%lf, %lf, %lf)\n",
        satellites[i] -> symbol,
        satellites[i] -> pos_x,
        satellites[i] -> pos_y,
        satellites[i] -> pos_z,
        satellites[i] -> theta_x,
        satellites[i] -> theta_y,
        satellites[i] -> theta_z
    ); // */
  }

  return satellites;
}

