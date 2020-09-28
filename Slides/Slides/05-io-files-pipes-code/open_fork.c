// fork_write.c: demostrate how a forked process inherits file
// descriptors. Experiment with interchanging order of open() and
// fork() system calls.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define COUNT 16

int main(int argc, char *argv[]){
  char *outfile = "shared.txt";                       
  system("touch shared.txt");                    // makes sure this file exists
  printf("output will be in '%s'\n",outfile);

  // ORDER of these two lines matters a lot, experiment with changing them
  int fd = open(outfile,O_WRONLY);               // open a file
  int child = fork();                            // fork a child 

  // both parent and child ...
  char buf[512];                
  snprintf(buf,512,"I'm process %d\n",getpid()); // format a message
  int nbytes = strlen(buf);
  write(fd, buf, nbytes);                        // write message to file

  close(fd);                                     // close file
  return 0;
}
