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
struct Satellite{
  float theta_x, theta_y, theta_z;
  int pos_x, pos_y, pos_z;
  char * name;
  char symbol;
}typedef Satellite;
