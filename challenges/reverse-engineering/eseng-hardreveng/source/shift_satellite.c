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
#include "./connection_helper.h"

#define NO_SATELLITES 4
#define GRID_EDGE_SIZE 12
#define MAX_ANGLE 360.0


/**
 * Gets the satellite position information from all satellites and prints thier
 * positions witin three cartesian planes representing 3D space.
 *
 * parameters
 * ----------
 * conn (Conn_Info *) - The connection info to the satellites
 */
void get_satellite_positions(Conn_Info * conn);

/**
 * Gets the satellite orientation information from a satellite with a given
 * symbol. then print a nice terminal output that shows the satellite and its
 * orientation in space relative to the predefined coordinate grid.
 *
 * parameters
 * ----------
 * conn (Conn_Info *) - The connection info to the satellites
 * symbol (char) - The symbol of the desired satellite
 */
void get_satellite_orientation(Conn_Info * conn, char symbol);

/**
 * Fetches all information about a given satellite.
 *
 * Satellite information will be returned in the following format:
 *
 * ```
 * symbol1|x|y|z|theta_x|theta_y|theta_z[;symbol2|x|y|z (...)]
 * ```
 *
 * parameters
 * ----------
 * conn (Conn_Info *) - information about the connection
 * symbol (char) - the symbol to fetch. expects either a `*` for all satellites
 *                 or the symbol of an actual Satellite (A, B, C...)
 */
Satellite ** fetch_satellite_info(Conn_Info * conn, char symbol);

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
int axis_index_map(const Satellite * satellite, const char axis_label);

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
void print_grid(Satellite ** satellites, const char row_axis, const char column_axis);

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
void print_satellite_positions(Satellite ** satellites);

/**
 * Print the current orienation of each satellite using the x,y,z components of
 * the angle with respect to the overall positional grid.
 *
 * output should be formatted as such:
 * Symbol | Orientation (x,y,z)
 * L      | (theta_x, theta_y, theta_z)
 * M      | (theta_x, theta_y, theta_z)
 * N      | (theta_x, theta_y, theta_z)
 *
 * angles are provided in degrees.
 *
 * parameters
 * ----------
 * satellites (Satellite **): the array of satellite data to print from
 */
void print_satellite_orientations(Satellite ** satellites);

/**
 * Frees the memory containing satellite information
 */
void free_satellites(Satellite ** satellites);

int main(int argc, char ** argv){
    fprintf(stdout,
"   ///\n"\
"  ///                      Welcome to the\n"\
" /// /\\  .                ______                    ______   ___            ___  ___\n"\
"///\\/  \\/     \\\\\\        /     /           ___     /     /  /  /            \\  \\ \\  \\ +----+\n"\
"// /    \\  /   \\\\\\      /  ___/  ____   __/  /__  /  ___/  /  /     ______  /  / /  / |    |\n"\
"   \\    / //    \\\\\\     \\  \\    |___ \\ /_   ___/  \\  \\    /  /___  / __  / /  / /  /  |    |\n"\
"    \\  /\\///    ///   ___\\  \\  / _   /  /  /    ___\\  \\  /  _   / /  ___/ /  / /  /   |    |\n"\
"     \\/ ///    ///   /      / / /_/ /  /  /__  /      / /  //  / /  /__  /  / /  /    |    |\n"\
"       ///    ///   /______/ /____\\/   \\____/ /______/ /__//__/  \\____/  \\__\\ \\__\\    +----+\n"\
"       //           Maintenance Console\n"
    );
  // Satellite ** satellites = fetch_satellite_info();
  // print_satellite_positions(satellites);
  // print_satellite_orientations(satellites);
  Conn_Info *conn = init_connection();

  char command[7];
  while(1){
    fprintf(stdout, "awaiting input...\n");
    memset(command, '\0', 7);
    scanf("%6s", command);
    if(strlen(command) == 0) break; // close if EOF

    if(strncmp(command, "POSN", 4) == 0){
      get_satellite_positions(conn);
      continue;
    }

    if(strncmp(command, "ORNT", 4) == 0){
      get_satellite_orientation(conn, command[5]);
    }
    /*
    sig_send_msg(conn, command, 4);

    int msg_len = 0;
    char *secondtest = sig_lstn_msg(conn, &msg_len);
    printf("%s\n", secondtest);
    // */
  }

  close_connection(conn);
  return 0;
}

void get_satellite_positions(Conn_Info * conn){
  Satellite ** satellites = fetch_satellite_info(conn, '*');
  print_satellite_positions(satellites);
  free_satellites(satellites);
}

void get_satellite_orientation(Conn_Info * conn, char symbol){
  // an array of satellites, though there should only be one.
  Satellite ** satellites = fetch_satellite_info(conn, symbol);
  fprintf(stdout,
      "      .\n"\
      "|||  _|_  |||\n"\
      "||| |   | |||\n"\
      "|||-| %c |-|||\n"\
      "||| |___| |||\n"\
      "|||       |||\n"\
      "theta_x: %3.3lfdeg, theta_y: %3.3lfdeg, theta_z: %3.3lfdeg\n",
      satellites[0] -> symbol,
      satellites[0] -> theta_x,
      satellites[0] -> theta_y,
      satellites[0] -> theta_z
  );
  free_satellites(satellites);
}

Satellite ** fetch_satellite_info(Conn_Info * conn, char symbol){
  char fetch_command[7];
  sprintf(fetch_command, "ping:%c", symbol);
  sig_send_msg(conn, fetch_command, 6);

  int response_len;
  char * response = sig_lstn_msg(conn, &response_len);
  Satellite ** satellites = malloc(sizeof(void*) * NO_SATELLITES);
  memset(satellites, '\0', sizeof(void*) * NO_SATELLITES);

  // symbol1|x|y|z|theta_x|theta_y|theta_z[;symbol2|x|y|z (...)]
  char * resp_head = response;
  int sat_index = 0;
  while(*resp_head != '\0'){
    Satellite * new_sat = malloc(sizeof(Satellite));
    sscanf(resp_head,
      "%c|%d|%d|%d|%f|%f|%f",
      &(new_sat -> symbol),
      &(new_sat -> pos_x),
      &(new_sat -> pos_y),
      &(new_sat -> pos_z),
      &(new_sat -> theta_x),
      &(new_sat -> theta_y),
      &(new_sat -> theta_z)
    );

    satellites[sat_index] = new_sat;
    sat_index++;

    // move one character past ';' and avoid going to the nullbyte.
    resp_head++;
    while(*(resp_head-1) != ';') resp_head++;
    // fprintf(stderr, "%d  - 0x%x, (%p)\n", __LINE__, *resp_head, resp_head);
    if(*(resp_head) == '\0'){
      break;
    }
  }

  /*/ debugging
  fprintf(stderr, "stopped! (%p, NULL -> %p)\n", satellites[0], NULL);
  for(int i = 0; i < NO_SATELLITES; i++){
    if(satellites[i] == NULL) break;
    fprintf(stderr, "Satellite %c: pos:(%d, %d, %d), angle:(%lf, %lf, %lf)\n",
        satellites[i] -> symbol,
        satellites[i] -> pos_x,
        satellites[i] -> pos_y,
        satellites[i] -> pos_z,
        satellites[i] -> theta_x,
        satellites[i] -> theta_y,
        satellites[i] -> theta_z
    );
  }// */

  return satellites;
}

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

void print_grid(Satellite ** satellites, const char row_axis, const char column_axis){
  char * spacing = malloc(4 * GRID_EDGE_SIZE * sizeof(char)+1); // account for nullbyte
  memset(spacing, '\x00', 4*GRID_EDGE_SIZE+1);
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

  strcpy(bottom_label, bottom_label_base_start);
  strcat(bottom_label, spacing);
  strcat(bottom_label, bottom_label_base_end);

  // show the source and receiver along the y-axis
  if(column_axis == 'y'){
    const char * src_label = "[src]";
    const char * you_label = "[you]";
    // find halfway point and write 2 bytes before it. this works for even-length and
    // odd length strings
    const int axis_start_write = strlen(top_label) / 2 - 2;
    strncpy(top_label+axis_start_write, src_label, 5); // do not copy nullbyte
    strncpy(bottom_label+axis_start_write, you_label, 5); // do not copy nullbyte
  }

  printf("%s\n", top_label);

  const int replace_char = 1; // the index to write the symbol within the cell

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

void print_satellite_positions(Satellite ** satellites){
  char * border = malloc((5 + 4 * GRID_EDGE_SIZE) * sizeof(char)); // add 5 bc "+--" and "-+"
  strcpy(border, "+--");
  for(int i = 0; i < GRID_EDGE_SIZE; i++){
    strcat(border, "----");
  }
  strcat(border, "--+");

  print_grid(satellites, 'x', 'y');
  printf("%s\n", border);
  print_grid(satellites, 'z', 'y');
  printf("%s\n", border);
  print_grid(satellites, 'z', 'x');

  free(border);
}

void print_satellite_orientations(Satellite ** satellites){
  printf("Symbol | Orientation\n");
  for(int i = 0; i < NO_SATELLITES; i++){
    printf("   %c   | (%f, %f, %f)\n",
        satellites[i] -> symbol,
        satellites[i] -> theta_x,
        satellites[i] -> theta_y,
        satellites[i] -> theta_z
    );
  }
}

void free_satellites(Satellite ** satellites){
  int index = 0;
  while(index < NO_SATELLITES){
    if(satellites[index] == NULL) break;
    free(satellites[index]);
    index++;
  }

  free(satellites);
}
