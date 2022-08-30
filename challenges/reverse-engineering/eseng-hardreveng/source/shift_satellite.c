/**
 * shift_satellite.c
 * developed by e-seng for magpieCTF 2023
 *
 * this binary will act as the server backend of some client. inputs received
 * are expected to be encoded by the client and then interpreted here on the
 * server.
 *
 * between some command station and the receiving station where the challenger
 * is connected two are three satellites. due to time, the dishes of the
 * satellite have become misaligned. as such, to collect the secrets from the
 * source, the dishes must be realigned.
 *
 * in a "practical/applied" setting, a user would interact with the client program
 * which will then send this server binary what to do. thus, the server's inputs
 * are obfuscated for "simplified" transmission (or something)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NO_SATELLITES 4
#define GRID_EDGE_SIZE 12
#define MAX_ANGLE 360.0

struct Satellite{
  float theta_x, theta_y, theta_z;
  int pos_x, pos_y, pos_z;
  char * name;
  char symbol;
}typedef Satellite;

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
Satellite ** fetch_satellite_info(){
  // seed the random value
  srand(time(NULL));

  const char * base_name = "Satellite _"; // replace _ with an identifier
  const int REPLACE_INDEX = 10;

  const int PART_SIZE = GRID_EDGE_SIZE / NO_SATELLITES;
  int y_history[NO_SATELLITES-1]; // keep track of previous satellite y positions
  int z_history[NO_SATELLITES-1]; // keep track of previous satellite z positions

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

      for(int j = i; j > 0; j--) unique &= suggested_x != y_history[j];
    }while(!unique);
    satellites[i] -> pos_x = suggested_x;

    do{
      unique = 1;
      suggested_z =  rand() % GRID_EDGE_SIZE;

      for(int j = i; j > 0; j--) unique &= suggested_z != z_history[j];
    }while(!unique);
    satellites[i] -> pos_z = suggested_z;

    satellites[i] -> theta_x = rand() / (float) RAND_MAX * MAX_ANGLE;
    satellites[i] -> theta_y = rand() / (float) RAND_MAX * MAX_ANGLE;
    satellites[i] -> theta_z = rand() / (float) RAND_MAX * MAX_ANGLE;

    // set name and symbol
    satellites[i] -> symbol = 'A' + i;
    satellites[i] -> name = malloc(strlen(base_name) * sizeof(char));
    strcpy(satellites[i] -> name, base_name);
    satellites[i] -> name[REPLACE_INDEX] = satellites[i] -> symbol;
  }

  return satellites;
}

/**
 * Maps the character axis label and fetches the associated positional component
 * from the provided satellite struct.
 *
 * parameters
 * ----------
 * satellite (const Satellite *): the satellite struct to fetch the coordinate from
 * axis_label (const char): the name of the axis that needs to be fetched.
 *
 * note: x corresponds to pos_x and likewise for other axis
 * if an invalid value is provided, return something out of bounds
 */
int axis_index_map(const Satellite * satellite, const char axis_label){
  switch(axis_label){
    case('x'):
      return satellite -> pos_x;
    case('y'):
      return satellite -> pos_y;
    case('z'):
      return satellite -> pos_z;
  }

  return -1; // invalid axis given
}
int main(int argc, char ** argv){
  Satellite ** satellites = fetch_satellite_info();
}
