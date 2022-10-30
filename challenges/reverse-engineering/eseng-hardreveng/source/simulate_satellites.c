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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "./connection_helper.h"

#define NO_SATELLITES 4
#define GRID_EDGE_SIZE 12
#define MAX_ANGLE 360.0

struct Sat_Registers {
  short zp;
  short ip;
  short rp;
  short fp;
  short sp;
  short dx;
  short dy;
  short dz;
} typedef Sat_Registers;

/**
 * Prints either all satellites or the satellite with the specified symbol
 *
 * symbol1|x|y|z|theta_x|theta_y|theta_z[;symbol2|x|y|z (...)]
 */
void print_satellite_info(Satellite ** satellites, char symbol);

/**
 * Connect to a specified satellite with a given symbol and send machine code to
 * the satellite. machine code will follow the assembly as defined by the
 * processor schematic.
 *
 * parameters
 * ----------
 * satellites (Satellite **) - the array of all satellites
 * symbol (char) - the symbol of the satellite to connect to
 */
void send_sat_instructions(Satellite ** satellites, char symbol);

/**
 * Checks if the orientation of each satellite is correct. For every satellite
 * within the input array, satellites[i] should point to satellites[i+1]. this
 * function will return the number of properly connected satellites.
 *
 * parameters
 * ----------
 * satellites (Satellite **) - the array of satellites to check
 */
short check_satellite_connectivity(Satellite ** satellites);

/**
 * Checks if a value is within the modulated range.
 *
 * parameters
 * ----------
 * lower (double) - the lower bound of the range
 * upper (double) - the upper bound of the range
 * value (double) - the value to check
 * modulo (int) - the modulus
 */
short check_modulo_range(double lower, double upper, double value, int modulo);

/**
 * Reverses the endianness of the provided array in memory.
 *
 * parameters
 * ----------
 * buff (void *) - some pointer pointing to some data to reverse the endianness
 *                 of
 * size (int) - the size of the buffer to reverse the endianness
 */
void reverse_endianness(void * buff, int size);

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
 * Parse the next 16 bits that the void pointer points to as a float. This
 * converts the 16bits into a 32 bit float, and returns said float value.
 *
 * this will follow IEEE 754 floating point defintions
 * (I like using this website: http://evanw.github.io/float-toy/)
 *
 * parameters
 * ----------
 * b16_float_ptr (void *) - a pointer to 16 bits representing a floating point
 *                          value as defined by
 *
 * returns the 32 bit float that should be equivalent to the 16 bit float
 */
float parse_16b_float(void *);

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
    if(strlen(command) == 0) break;

    if(strncmp(command, "ping", 4) == 0){
      print_satellite_info(satellites, command[5]);
      continue;
    }
    if(strncmp(command, "conn", 4) == 0){
      send_sat_instructions(satellites, command[5]);
      continue;
    }

    if(strncmp(command, "bash", 4) == 0){
      if(check_satellite_connectivity(satellites) < NO_SATELLITES - 1){
        fprintf(stdout, "CONN_FAILED\n");
        continue;
      }
      fprintf(stdout, "CONN_SUCCESS\n");
      system("/bin/sh");
      continue;
    }
    fprintf(stdout, "UNAVAIL_CMD\n");
    fflush(stdout);
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

void send_sat_instructions(Satellite ** satellites, char symbol){
  Satellite * conn_sat = NULL;
  for(int i = 0; i < NO_SATELLITES; i++){
    if(satellites[i] -> symbol != symbol) continue;
    conn_sat = satellites[i];
    break;
  }

  if(conn_sat == NULL){ // no satellite could be connected to
    fprintf(stdout, "UNAVAIL_SAT\n");
    fflush(stdout);
    return;
  }
  fprintf(stdout, "CONN_CNFRM\n");
  fflush(stdout);

  Sat_Registers registers;
  char instructions[136];
  memset(&registers, 0, sizeof(Sat_Registers));
  memset(instructions, 0, 136);

  // scanf("%128s", instructions);
  // Read instruction input
  int offset = 0;
  do{
    offset += read(0, instructions+offset, 8);
  }while(offset < 128 && instructions[offset-1] != '\n');
  instructions[offset-1] = '\0';

  reverse_endianness(instructions, 128);

  /*/ debugging
  for(int i = 0; i < (32/sizeof(short)); i++){
    fprintf(stderr, "%4x\n", ((short*)instructions)[i]);
  }// */

  // run instructions :p
  short * instructions_bin = (short*)instructions;
  do{
    // load instruction
    short instruction = instructions_bin[registers.ip];

    // get source and destination registers
    short * rs, * rd, rs_index, rd_index, arg, immediate;
    rs_index = (instruction >> 11) & 0x7;
    rd_index = (instruction >> 8) & 0x7;
    arg = (instruction >> 14) & 0x3;
    immediate = instruction & 0xff;

    switch(rs_index){
      case(0):
        rs = &(registers.zp);
        break;
      case(5):
        rs = &(registers.dx);
        break;
      case(6):
        rs = &(registers.dy);
        break;
      case(7):
        rs = &(registers.dz);
        break;
      default:
        fprintf(stderr, "bad rs addr: %d\n", rs_index);
        fprintf(stdout, "ABRT_SEGFAULT\n");
        fflush(stdout);
        return;
    }

    switch(rd_index){
      case(5):
        rd = &(registers.dx);
        break;
      case(6):
        rd = &(registers.dy);
        break;
      case(7):
        rd = &(registers.dz);
        break;
      default:
        fprintf(stderr, "bad rd addr: %d\n", rd_index);
        fprintf(stdout, "ABRT_SEGFAULT\n");
        fflush(stdout);
        return;
    }

    /*/ debug
    fprintf(stderr,
        "--\n"\
        "arg: %d (%x)\n"\
        "imm: %d (%x)\n",
        arg, arg, immediate, immediate
    );
    // */
    // execute instruciton
    switch(arg){
      case(0): // addi
        *rd = *rs + immediate;
        break;
      case(1): // ori
        *rd = *rs | immediate;
        break;
      case(2): // sll
        *rd = *rd << immediate;
        break;
      case(3): // lui
        *rd &= 0x00ff;
        *rd |= immediate << 8;
        break;
      default:
        fprintf(stderr, "bad arg: %d\n", arg);
        fprintf(stdout, "ABRT_SEGFAULT\n");
        fflush(stdout);
        return;
    }

    registers.ip++; // move to next instruction
  }while(instructions_bin[registers.ip]); // halt when no instruction remains

  // debug the current register values
  fprintf(
      stderr,
      "[debug] registers:\n"\
      "      | zp: %d (%x)\n"\
      "      | ip: %d (%x)\n"\
      "      | rp: %d (%x)\n"\
      "      | fp: %d (%x)\n"\
      "      | sp: %d (%x)\n"\
      "      | dx: %d (%x)\n"\
      "      | dy: %d (%x)\n"\
      "      | dz: %d (%x)\n",
      registers.zp, registers.zp,
      registers.ip, registers.ip,
      registers.rp, registers.rp,
      registers.fp, registers.fp,
      registers.sp, registers.sp,
      registers.dx, registers.dx,
      registers.dy, registers.dy,
      registers.dz, registers.dz
  );
  // */

  add_to_orientation(
      conn_sat,
      parse_16b_float(&registers.dx),
      'x'
  );
  add_to_orientation(
      conn_sat,
      parse_16b_float(&registers.dy),
      'y'
  );
  add_to_orientation(
      conn_sat,
      parse_16b_float(&registers.dz),
      'z'
  );

  fprintf(stdout, "INSTRUCTIONS_COMPL\n");
}

short check_satellite_connectivity(Satellite ** satellites){
  short no_connected = 0;

  for(int i = 0; i < NO_SATELLITES - 1; i++){
    /* calculate the angles for the vector S_i->S_i+1. this is equal to
     *                    _________________________________
     *                  ./(z_i+1 - z_i)^2 + (y_i+1 - y_i)^2
     * theta_x = arctan(-----------------------------------)
     *                              x_i+1 - x_i
     *
     * and similar for the other angular components
     */
    double
      true_theta_x = atan(
        sqrt(
          pow(satellites[i+1] -> theta_y - satellites[i] -> theta_y, 2) +
          pow(satellites[i+1] -> theta_z - satellites[i] -> theta_z, 2)
          ) /
        (satellites[i+1] -> theta_x - satellites[i] -> theta_x)),
      true_theta_y = atan(
        sqrt(
          pow(satellites[i+1] -> theta_x - satellites[i] -> theta_x, 2) +
          pow(satellites[i+1] -> theta_z - satellites[i] -> theta_z, 2)
          ) /
        (satellites[i+1] -> theta_y - satellites[i] -> theta_y)),
      true_theta_z = atan(
        sqrt(
          pow(satellites[i+1] -> theta_y - satellites[i] -> theta_y, 2) +
          pow(satellites[i+1] -> theta_x - satellites[i] -> theta_x, 2)
          ) /
        (satellites[i+1] -> theta_z - satellites[i] -> theta_z));

    true_theta_x = ((int)true_theta_x % 360) + (true_theta_x - (int)true_theta_x);
    if(true_theta_x < 0) true_theta_x += 360;
    true_theta_y = ((int)true_theta_y % 360) + (true_theta_y - (int)true_theta_y);
    if(true_theta_y < 0) true_theta_y += 360;
    true_theta_z = ((int)true_theta_z % 360) + (true_theta_z - (int)true_theta_z);
    if(true_theta_z < 0) true_theta_z += 360;

    // check if the orientation of satellites[i] is within range of error
    int uncertainty = 5; // degrees
    fprintf(stderr, "true angles: %f, %f, %f\n", true_theta_x, true_theta_y, true_theta_z);
    if(!check_modulo_range(
          (int)(true_theta_x + 360 - uncertainty) % 360 + true_theta_x - (int)true_theta_x,
          (int)(true_theta_x + 360 + uncertainty) % 360 + true_theta_x - (int)true_theta_x,
          satellites[i] -> theta_x,
          MAX_ANGLE)){
      fprintf(stderr, "[debug] %d: failed on theta_x of sat %c, range (%f, %f)\n",
          __LINE__,
          satellites[i] -> symbol,
          ((int)(true_theta_x + 360) - uncertainty) % 360 + true_theta_x - (int)true_theta_x,
          ((int)(true_theta_x + 360) + uncertainty) % 360 + true_theta_x - (int)true_theta_x
      );
      continue;
    }

    if(!check_modulo_range(
          (int)(true_theta_y + 360 - uncertainty) % 360 + true_theta_y - (int)true_theta_y,
          (int)(true_theta_y + 360 + uncertainty) % 360 + true_theta_y - (int)true_theta_y,
          satellites[i] -> theta_y,
          MAX_ANGLE)){
      fprintf(stderr, "[debug] %d: failed on theta_y of sat %c, range (%f, %f)\n",
          __LINE__,
          satellites[i] -> symbol,
          (int)(true_theta_y + 360 - uncertainty) % 360 + true_theta_y - (int)true_theta_y,
          (int)(true_theta_y + 360 + uncertainty) % 360 + true_theta_y - (int)true_theta_y
      );
      continue;
    }

    if(!check_modulo_range(
          ((int)(true_theta_z + 360) - uncertainty) % 360 + true_theta_z - (int)true_theta_z,
          ((int)(true_theta_z + 360) + uncertainty) % 360 + true_theta_z - (int)true_theta_z,
          satellites[i] -> theta_z,
          MAX_ANGLE)){
      fprintf(stderr, "[debug] %d: failed on theta_z of sat %c, range (%f, %f)\n",
          __LINE__,
          satellites[i] -> symbol,
          ((int)(true_theta_z + 360) - uncertainty) % 360 + true_theta_z - (int)true_theta_z,
          ((int)(true_theta_z + 360) + uncertainty) % 360 + true_theta_z - (int)true_theta_z
      );
      continue;
    }
    /*/
    if((int)(true_theta_x + 360 - uncertainty) % 360 + true_theta_x - (int)true_theta_x >
        (int)(satellites[i] -> theta_x + 360 + uncertainty) % 360 +
        satellites[i] -> theta_x - (int) satellites[i] -> theta_x){
      fprintf(stderr, "[debug] %d: failed on theta_x of sat %c, range (%f, %f)\n",
          __LINE__,
          satellites[i] -> symbol,
          ((int)(true_theta_x + 360) - uncertainty) % 360 + true_theta_x - (int)true_theta_x,
          ((int)(true_theta_x + 360) + uncertainty) % 360 + true_theta_x - (int)true_theta_x
      );
      continue;
    }
    if(((int)(true_theta_y + 360) - uncertainty) % 360 + true_theta_y - (int)true_theta_y >
        satellites[i] -> theta_y ||
        ((int)(true_theta_y + 360) + uncertainty) % 360 + true_theta_y - (int)true_theta_y <
        satellites[i] -> theta_y){
      fprintf(stderr, "[debug] %d: failed on theta_y of sat %c, range (%f, %f)\n",
          __LINE__,
          satellites[i] -> symbol,
          (int)(true_theta_y + 360 - uncertainty) % 360 + true_theta_y - (int)true_theta_y,
          (int)(true_theta_y + 360 + uncertainty) % 360 + true_theta_y - (int)true_theta_y
      );
      continue;
    }
    if((int)(true_theta_z + 360 - uncertainty) % 360 + true_theta_z - (int)true_theta_z >
        satellites[i] -> theta_z ||
        (int)(true_theta_z + 360 + uncertainty) % 360 + true_theta_z - (int)true_theta_z <
        satellites[i] -> theta_z){
      fprintf(stderr, "[debug] %d: failed on theta_z of sat %c, range (%f, %f)\n",
          __LINE__,
          satellites[i] -> symbol,
          ((int)(true_theta_z + 360) - uncertainty) % 360 + true_theta_z - (int)true_theta_z,
          ((int)(true_theta_z + 360) + uncertainty) % 360 + true_theta_z - (int)true_theta_z
      );
      continue;
    } // */

    no_connected++;
  }

  return no_connected;
}

short check_modulo_range(double lower, double upper, double value, int modulo){
  /**
   * three cases arrive where the value may be in range:
   * 1. lower <= value <= upper - make this check normally
   * 2. value <= upper <= lower - lower has been modulated to a greater value
   *  -> in this case: subtract lower by modulo and check again
   * 3. upper <= lower <= value - upper has been modulated to a lower value
   *  -> in this case: add upper by modulo and check again
   */

  fprintf(stderr, "[debug] %i found lower = %lf, upper = %lf, value = %lf : mod = %i\n", __LINE__,
      lower,
      upper,
      value,
      modulo);

  if(lower <= value && value <= upper)
    return 1;
  if(upper <= lower && value <= lower)
    return check_modulo_range(lower-modulo, upper, value, modulo);
  if(upper <= lower && lower <= value)
    return check_modulo_range(lower, upper+modulo, value, modulo);

  return 0;

}

void reverse_endianness(void * buff, int size){
  /*/ Swap the order between half-words
  short * short_buff_ptr = ((short*) buff);
  for(int i = 1; i < size; i+=sizeof(short)){
    short_buff_ptr[i-1] = short_buff_ptr[i-1] ^ short_buff_ptr[i];
    short_buff_ptr[i] = short_buff_ptr[i-1] ^ short_buff_ptr[i];
    short_buff_ptr[i-1] = short_buff_ptr[i-1] ^ short_buff_ptr[i];
  } // this part is only useful for 32 bit things */

  // Swap the order between bytes
  char * char_buff_ptr = ((char *) buff);
  for(int i = 1; i < size; i+=2*sizeof(char)){
    char_buff_ptr[i-1] = char_buff_ptr[i-1] ^ char_buff_ptr[i];
    char_buff_ptr[i] = char_buff_ptr[i-1] ^ char_buff_ptr[i];
    char_buff_ptr[i-1] = char_buff_ptr[i-1] ^ char_buff_ptr[i];
  }// */
}

void add_to_orientation(Satellite * satellite, float delta_angle, char component){
  switch(component){
    case('x'):
      satellite -> theta_x += delta_angle;
      satellite -> theta_x = ((int)satellite -> theta_x % 360) + (satellite -> theta_x - (int)satellite -> theta_x);
      if(satellite -> theta_x < 0) satellite -> theta_x += 360;
      break;
    case('y'):
      satellite -> theta_y += delta_angle;
      satellite -> theta_y = ((int)satellite -> theta_y % 360) + (satellite -> theta_y - (int)satellite -> theta_y);
      if(satellite -> theta_y < 0) satellite -> theta_y += 360;
      break;
    case('z'):
      satellite -> theta_z += delta_angle;
      satellite -> theta_z = ((int)satellite -> theta_z % 360) + (satellite -> theta_z - (int)satellite -> theta_z);
      if(satellite -> theta_z < 0) satellite -> theta_z += 360;
      break;
    default:
      fprintf(stderr, "[error] Incorrect orientation received at satellite\n");
  }
}

float parse_16b_float(void * b16_float_ptr) {
  int b32_float_bin = 0;
  memset(&b32_float_bin, 0, sizeof(float));
  // extend the data from 16 bits to 32 bits
  unsigned int b16_float_bin = (unsigned int) *((short *) b16_float_ptr);
  if(b16_float_bin == 0) return 0.0; // account for a float of 0
  // copy sign+exp sign bit
  *((int *)&b32_float_bin) = (int) ((b16_float_bin << 16) & 0xc0000000);
  // pad exponent depending on the exponent sign bit
  if(!(0x4000 & b16_float_bin)) {
    b32_float_bin |= 0x38000000;
  }
  // copy remaining bits
  b32_float_bin |= (b16_float_bin << 13) & 0x07FFFFFF;

  return *((float*)&b32_float_bin);
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

