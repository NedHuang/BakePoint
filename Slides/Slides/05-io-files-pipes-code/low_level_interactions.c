// low_level_interactions.c: Demonstrate low-level interaction with
// user with read/write, much more painful than standard C functions

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 128

int main(int argc, char *argv[]){
  char buffer[BUFSIZE] = "Wubba lubba dub dub!\n";
  int length = strlen(buffer);
  write(STDOUT_FILENO, buffer, length);                     // must write corrent number of bytes

  snprintf(buffer, BUFSIZE, "Enter # Mortys: ");            // create output buffer
  write(STDOUT_FILENO, buffer, strlen(buffer));             // write to the screen

  int bytes_read = read(STDIN_FILENO, buffer, BUFSIZE-1);   // read into buffer
  buffer[bytes_read] = '\0';                                // null-terminate string read
  int mortys = atoi(buffer);                                // use atoi() to convert to inteber

  snprintf(buffer, BUFSIZE, "%d Mortys requested\n",mortys);// output message
  write(STDOUT_FILENO, buffer, strlen(buffer));             // write to screen

  return 0;
}
