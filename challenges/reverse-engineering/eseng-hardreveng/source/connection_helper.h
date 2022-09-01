/**
 * connection_helper.h
 * developed by e-seng for magpieCTF 2023
 *
 * header file for connection_header.c
 *
 * this shared binary will contain functions that will make shift_satellite seem
 * like it is genuinely sending signals through some device.
 *
 * ideally, this library binary will start a simulate_satellites process and
 * enforce communication between the two of them... somehow lol.
 */

#ifndef CONN_HELPER
#define CONN_HELPER
#define DEVICE_NAME "0MN1-F746S-43V3R" // fun device name to print :)
#define SIG_MSG_FAIL 0

// this Satellite struct will represent on-board data of a satellite
struct Satellite{
  float theta_x, theta_y, theta_z;
  int pos_x, pos_y, pos_z;
  char * name;
  char symbol;
  pid_t connection_id;
}typedef Satellite;


/**
 * Initializes the "connection" between the server and the satellites.
 *
 * in reality, this function will spawn a simulate_satellites process that
 * the server will communicate with along with any tools required to ensure
 * bidirectional communication.
 */
void init_connection();

/**
 * Closes the "connection" between the server and the satellites.
 *
 * in reality, this function will clean up any generated files or used memory
 * and close the simulate_satellites function.
 */
void close_connection();

/**
 * Sends a message between a satellite and the server.
 *
 * in reality, this function will write to the allocated fifo file or allocated
 * memory to be read by the other process.
 *
 * returns an integer depending on whether the signal could be sent
 * SIG_MSG_FAIL: indicates that the message could not be sent
 * any integer > 0: indicates the number of bytes sent
 *
 * parameters
 * ----------
 * msg (const char *): the message to send
 * msg_len (int): the length of the message
 */
int sig_send_msg(const char * msg, int msg_len);

/**
 * Waits for and reads messages between a satellite the server.
 *
 * in reality, this function will read from the allocated fifo file or allocated
 * memory until a newline or nullbyte is presented.
 *
 * as such, all messages must be contained within one line.
 *
 * returns an integer depending on whether the signal could be sent
 * SIG_MSG_FAIL: indicates that the message could not be read
 * any integer > 0: indicates the number of bytes read
 *
 * parameters
 * ----------
 * buff (char *): the buffer to receive the message within
 */
int sig_lstn_msg(char *);

#endif
