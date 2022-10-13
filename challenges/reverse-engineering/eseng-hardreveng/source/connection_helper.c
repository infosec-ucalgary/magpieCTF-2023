/**
 * connection_helper.c
 * developed by e-seng for magpieCTF 2023
 *
 * the implementation of connection_helper.h
 *
 * see that file for additional details
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // need the ability to fork a process
#include "connection_helper.h"
#define SIG_MSG_MAX 1024 // bytes, no message should be this long.

pid_t init_connection(){
  // create file descriptors to read/write to connect to simulate_satellites.c
  // fd[0] -> write
  // fd[1] -> read
  int fd[2];
  int pipe_check = pipe(fd);

  if(pipe_check < 0){
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
    dup2(fd[0], 0); // set stdin of child
    dup2(fd[1], 1); // set stdout of child

    execve("./build/simulate_satellites", NULL, NULL);
    exit(0);
  }

  // parent process
  return sim_pid;
}

void close_connection(pid_t sim_proc){
}

int sig_send_msg(const char * msg, int msg_len){
  return 0;
}

int sig_lstn_msg(char * buff){
  return 0;
}
