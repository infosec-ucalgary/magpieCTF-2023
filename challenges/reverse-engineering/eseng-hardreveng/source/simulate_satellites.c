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
 * Prints either all satellites or the satellite with the specified symbol
 *
 * symbol1|x|y|z|theta_x|theta_y|theta_z[;symbol2|x|y|z (...)]
 */
void print_satellite_info(Satellite ** satellites, char symbol);

/**
 * Adds a float value to the angle of where the satellite is pointed, ie. its
 * orientation. The specified angle should only be added to the specified
 * component, represented by a 'x', 'y' or 'z' character. the satellite to
 * update is also passed as an argument
 *
 * parameters
 * ----------
 * satellite (Satellite) - the satellite that should be updated
 * delta_angle (float) - the angle to which the satellite orientation should be
 *                       moved
 * component (char) - the character representing which component should be
 *                    updated
 */
void add_to_orientation(Satellite * satellite, float delta_angle, char component);

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
  char command[7]; // max command length is 4 + an optional symbol
  fprintf(stderr, "[info] connection recieved\n");

  Satellite ** satellites = generate_satellite_info();

  while(1){
    memset(command, 0, 7);
    scanf("%6s", command);
    // fprintf(stderr, "request: %s\n", command);
    // fprintf(stdout, "response!!\n");
    // fflush(stdout);
    if(strncmp(command, "ping", 4) == 0)
      print_satellite_info(satellites,
        command[5]);
    else{
        fprintf(stdout, "UNAVAIL_CMD\n");
        fflush(stdout);
    }
  }
}

void print_satellite_info(Satellite ** satellites, char symbol){
  char * satellite_info_buf = malloc(NO_SATELLITES * 40 * sizeof(char));
  memset(satellite_info_buf, '\0', 40 * NO_SATELLITES * sizeof(char));

  for(int index = 0; index < NO_SATELLITES; index++){
    if(satellites[index] -> symbol != symbol && symbol != '*') continue;

    char satellite_str_tmp[40];
    memset(satellite_str_tmp, '\0', 40);
    sprintf(satellite_str_tmp,
        "%c|%00d|%00d|%00d|%3.3lf|%3.3lf|%3.3lf;",
        satellites[index] -> symbol,
        satellites[index] -> pos_x,
        satellites[index] -> pos_y,
        satellites[index] -> pos_z,
        satellites[index] -> theta_x,
        satellites[index] -> theta_y,
        satellites[index] -> theta_z
    );
    strcat(satellite_info_buf, satellite_str_tmp);
  }

  fprintf(stdout, "%s\n", satellite_info_buf);
  fflush(stdout);
  free(satellite_info_buf);
}

void add_to_orientation(Satellite * satellite, float delta_angle, char component){
  switch(component){
    case('x'):
      satellite -> theta_x += delta_angle;
      break;
    case('y'):
      satellite -> theta_y += delta_angle;
      break;
    case('z'):
      satellite -> theta_z += delta_angle;
      break;
    default:
      fprintf(stderr, "[error] Incorrect orientation received at satellite\n");
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

