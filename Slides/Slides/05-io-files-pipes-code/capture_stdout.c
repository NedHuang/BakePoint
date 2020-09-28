// Redirect standard output temporarily to a pipe, then restore
// standard output and retrieve the captured output
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PREAD 0                           // index of read end of pipe
#define PWRITE 1                          // index of write end of pipe

int main(){
  setvbuf(stdout, NULL, _IONBF, 0);       // Turn off buffering, try commenting

  printf("Piping\n");
  int my_pipe[2];
  pipe(my_pipe);                          // create a pipe

  int stdout_bak = dup(STDOUT_FILENO);    // create a backup of stdout
  dup2(my_pipe[PWRITE],STDOUT_FILENO);    // redirect stdout to pipe
  printf("In the my_pipe, five by five"); // won't appear on screen
  dup2(stdout_bak, STDOUT_FILENO);        // restore stdout
  close(my_pipe[PWRITE]);                 // done writing to pipe

  printf("Reading from my_pipe\n");
  char buf[2048];
  int bytes_read =                        // read from pipe
    read(my_pipe[PREAD], buf, 2048-1);
  buf[bytes_read] = '\0';                 // null terminate string
  close(my_pipe[PREAD]);                  // done reading from pipe
  printf("Read: '%s'\n",buf);
  
  return 0;
}
