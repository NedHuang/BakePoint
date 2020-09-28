// dup2_demo.c: demostrate use of dup2() to get a child to print into
// a file using printf; solution to in-class exercise
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
  system("touch write.txt");           // ensure file exists
  int fd = open("write.txt",O_WRONLY); // open a file
  pid_t child = fork();                // fork a child
  if(child == 0){                      // child section
    int backup = dup(STDOUT_FILENO);   // make backup of stdout
    dup2(fd,STDOUT_FILENO);            // use dup2 so child printf() goes into file, not screen
    printf("I'm the child\n");         // printf() now goes to open file
    fflush(stdout);                    // flush stdout buffers into file
    dup2(backup,STDOUT_FILENO);        // restore stdout
  }
  else{                                // parent section
    printf("I'm the parent\n");
  }
  printf("all done\n");                // both parent and child
  close(fd);
  return 0;
}


