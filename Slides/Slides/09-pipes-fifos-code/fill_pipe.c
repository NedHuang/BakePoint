// Demonstrates fillling a pipe which only occasionally gets read from
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]){
  int pip[2];
  pipe(pip);
  
  pid_t pid = fork();
  if(pid == 0){                       // CHILD
    char buf[BUFSIZE];                
    while(1){
      sleep(1);                       // sleep then...
      printf("Reading ...\n");
      fflush(stdout);
      read(pip[0], buf, BUFSIZE);     // empty the pipe a bit
    }
  }
  else{                               // PARENT
    for(int i=1; ; i++){              // keep writing to the pipe
      char c = '-';
      write(pip[1], &c, 1);           // blocks when pipe is full
      printf("Wrote %d so far\n",i);
      fflush(stdout);
    }
  }
  return 0;
}
