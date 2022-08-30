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

/**
 * Prints a 2-axis grid of some side view and places the satellites accordingly
 * within that grid.
 *
 * the grid will look like this, but scaled according to GRID_EDGE_SIZE
 * | row [src]     |
 * |  [ ] [ ] [ ]  |
 * |  [ ] [ ] [ ]  |
 * |  [ ] [ ] [ ]  |
 * | 0   [you] col |
 *
 * parameters
 * ----------
 * satellites (Satellite **): the array of satellites to populate the grid
 *                                  with
 * row_axis (const char): the character representing the axis name along the rows
 * column_axis (const char): the character respresenting the axis name along the columns
 */
void print_grid(Satellite ** satellites, const char row_axis, const char column_axis){
  char * spacing = malloc(4 * GRID_EDGE_SIZE * sizeof(char));
  memset(spacing, '\x00', 4*GRID_EDGE_SIZE);
  for(int i = 0; i < GRID_EDGE_SIZE; i++){
    strcat(spacing, "    ");
  }

  char top_label_base_start[4] = "| d";
  const char * edge_border_start = "|  ";
  const char * bottom_label_base_start = "| 0";
  const char * top_label_base_end = "  |";
  const char * edge_border_end = "  |";
  char bottom_label_base_end[4] = "d |";
  const char * cell_base = "[ ] ";

  top_label_base_start[2] = column_axis;
  bottom_label_base_end[0] = row_axis;

  char * top_label = malloc((5 + 4 * GRID_EDGE_SIZE + 1)* sizeof(char)); // +5 for edge stull and +1 for null byte :)
  char * bottom_label = malloc((5 + 4 * GRID_EDGE_SIZE + 1)* sizeof(char));
  strcpy(top_label, top_label_base_start);
  strcat(top_label, spacing);
  strcat(top_label, top_label_base_end);
  printf("%s\n", top_label);

  strcpy(bottom_label, bottom_label_base_start);
  strcat(bottom_label, spacing);
  strcat(bottom_label, bottom_label_base_end);

  const int replace_char = 1;

  char * print_row = malloc((5 + 4 * GRID_EDGE_SIZE + 1) * sizeof(char));
  char * cell = malloc(5 * sizeof(char));

  for(int row = 0; row < GRID_EDGE_SIZE; row++){
    memset(print_row, '\x00', 6+4*GRID_EDGE_SIZE);
    strcat(print_row, edge_border_start);
    for(int column = 0; column < GRID_EDGE_SIZE; column++){
      memset(cell, '\x00', 5);
      strcpy(cell, cell_base);

      for(int i = 0; i < NO_SATELLITES; i++){
        Satellite * s = satellites[i];
        if(axis_index_map(s, row_axis) != column) continue;
        if(axis_index_map(s, column_axis) != GRID_EDGE_SIZE-row-1) continue;
        cell[replace_char] = s -> symbol;
      }

      strcat(print_row, cell);
    }
    strcat(print_row, edge_border_end);
    printf("%s\n", print_row);
  }
  printf("%s\n", bottom_label);

  free(print_row);
  free(spacing);
  free(top_label);
  free(bottom_label);
  free(cell);
}

/**
 * Print the current positions of the satellites in a 3-perspective view.
 *
 * views will be printed out as such:
 *
 * x >, y ^
 * z >, y ^
 * z >, y ^
 *
 * the grid will look like this, but scaled according to GRID_EDGE_SIZE
 * | y   [src]     |
 * |  [ ] [ ] [ ]  |
 * |  [ ] [ ] [ ]  |
 * |  [ ] [ ] [ ]  |
 * | 0   [you]   x |
 * +---------------+
 * | y   [src]     |
 * |  [ ] [ ] [ ]  |
 * |  [ ] [ ] [ ]  |
 * |  [ ] [ ] [ ]  |
 * | 0   [you]   z |
 * +---------------+
 * | x             |
 * |  [ ] [ ] [ ]  |
 * |  [ ] [ ] [ ]  |
 * |  [ ] [ ] [ ]  |
 * | 0           z |
 */
void print_satellite_positions(Satellite ** satellites){
  char * border = malloc((5 + 4 * GRID_EDGE_SIZE) * sizeof(char)); // add 5 bc "+--" and "-+"
  strcpy(border, "+--");
  for(int i = 0; i < GRID_EDGE_SIZE; i++){
    strcat(border, "----");
  }
  strcat(border, "-+");

  print_grid(satellites, 'x', 'y');
  printf("%s\n", border);
  print_grid(satellites, 'z', 'y');
  printf("%s\n", border);
  print_grid(satellites, 'z', 'x');

  free(border);
}

int main(int argc, char ** argv){
  Satellite ** satellites = fetch_satellite_info();
  print_satellite_positions(satellites);
}
