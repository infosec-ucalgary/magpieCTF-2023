/**
 * connection_helper.c
 * developed by e-seng for magpieCTF 2023
 *
 * the implementation of connection_helper.h
 *
 * see that file for additional details
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // need the ability to fork a process
#include "connection_helper.h"
#define SIG_MSG_MAX 1024 // bytes, no message should be this long.

Conn_Info * init_connection(){
  // create file descriptors to read/write to connect to simulate_satellites.c
  // fd[0] -> write
  // fd[1] -> read
  int pipe_stdin[2], pipe_stdout[2];
  int pipe_check_in = pipe(pipe_stdin);
  int pipe_check_out = pipe(pipe_stdout);

  if(pipe_check_in != 0 || pipe_check_out != 0){
    printf("[err] Could not initialize communication to satellite.\n");
    printf("[err] Please try again\n");
    printf("[err] Contact administrator if this issue persists\n");
    printf("[err] Error: PIPE_CREATE_FAIL");
    exit(1);
  }

  pid_t sim_pid = fork();

  if(sim_pid < 0){
    printf("[err] Could not initialize communication to satellite.\n");
    printf("[err] Please try again\n");
    printf("[err] Contact administrator if this issue persists\n");
    printf("[err] Error: SIG_FORK_FAILED");
    exit(1);
  }

  if(sim_pid == 0){
    // child process, start simulation process
    close(pipe_stdin[1]); // no need to write to this process's stdin
    close(pipe_stdout[0]); // no need to read from this process's stdout

    dup2(pipe_stdin[0], 0); // read from stdin pipe
    dup2(pipe_stdout[1], 1); // write to stdout pipe

    execl("./build/simulate_satellites", NULL, NULL);
    // exit(0);
  }

  // parent process
  close(pipe_stdout[1]);
  close(pipe_stdin[0]);

  Conn_Info *conn = malloc(sizeof(Conn_Info));
  conn -> id = sim_pid;
  conn -> conn_stdout_fd = pipe_stdout[0]; // read from here
  conn -> conn_stdin_fd = pipe_stdin[1]; // write to here

  sleep(1); // block the parent, to ensure child is initialized
  return conn;
}

void close_connection(Conn_Info *conn){
  kill(conn -> id, SIGKILL);
  free(conn);
}

int sig_send_msg(Conn_Info *conn, const char * msg, int msg_len){
  int offset = 0;
  fprintf(stdout, "[info] sending data... 000 bytes");

  while(offset < msg_len){
    offset += write(conn -> conn_stdin_fd, msg+offset, msg_len);

    fprintf(stdout, "\r[info] sending data... %03d bytes", offset);
    fflush(stdout);
  }
  fprintf(stdout, "\n");

  sleep(1);
  return offset;
}

char * sig_lstn_msg(Conn_Info *conn, int * msg_len){
  const int BLOCK_SIZE = 64;
  const int READ_SPEED = 32;
  char * msg = malloc(BLOCK_SIZE * sizeof(char));
  int offset = 0, max_len = BLOCK_SIZE;

  fprintf(stdout, "[info] recieving data... 000 bytes");

  while(msg[offset-1] != '\n' && offset > -1){
    sleep(1);
    offset += read(conn -> conn_stdout_fd, msg+offset, READ_SPEED);

    fprintf(stdout, "\r[info] recieving data... %03d bytes", offset);
    fflush(stdout);
    if(msg[offset-1] == '\n'){
      msg[offset-1] = '\0';
      break;
    }

    if(offset == max_len){ // when the offset is the max_len, we need more memory
      max_len += BLOCK_SIZE;
      msg = realloc(msg, max_len);
    }
  }

  fprintf(stdout, "\n");

  if(offset < 0){
    printf("[err] Could not recieve data from satellite.\n");
    printf("[err] Please try again\n");
    printf("[err] Contact administrator if this issue persists\n");
    printf("[err] Error: SAT_READ_FAIL");
    exit(1);
  }

  *msg_len = offset;
  return msg;
}

extern void sig_spawn_shell(Conn_Info * conn){
  sig_send_msg(conn, "bash\n\0", 6);

  int response_len;
  char * response = sig_lstn_msg(conn, &response_len);

  fprintf(stdout, "[info] satellite response %s\n", response);
  if(strncmp(response, "CONN_FAILED", 11) == 0){
    fprintf(stdout,
        "[eror] shell process could not be started.\n" \
        "     | satellites are currently misaligned. please complete network.\n"
    );
    return;
  }
  free(response);

  fprintf(stdout, "[info] establishing shell process\n");
  system("/bin/bash -c \"source ~/.bashrc\"");
}
