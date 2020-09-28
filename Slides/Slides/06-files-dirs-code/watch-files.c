// Broken example of trying to watch 4 files using select. select() on
// files often returns "ready" but subsequent read() calls give 0
// bytes.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>

#define MAXFILES 4

int main(int argc, char *argv[]) {

  char *filenames[MAXFILES] = {
    "file0.txt",
    "file1.txt",
    "file2.txt",
    "file3.txt",
  };


  int fds[MAXFILES] = {};
  int maxfd = 0;
  int flags = O_RDONLY | O_CREAT;
  mode_t perms = S_IRUSR | S_IWUSR;
  for(int i=0; i<MAXFILES; i++){
    fds[i] = open(filenames[i],flags,perms);
    if(fds[i] == -1){
      perror("Couldn't open file");
      exit(1);
    }
    maxfd = (maxfd < fds[i]) ? fds[i] : maxfd;
  }


  printf("Waiting for input from %d files\n",MAXFILES);

  char buf[1024];
  while(1){
    fd_set read_set;
    FD_ZERO(&read_set);
    for(int i=0; i<MAXFILES; i++){
      FD_SET(fds[i], &read_set);
    }

    struct timeval wait_time;
    wait_time.tv_sec = 5;
    wait_time.tv_usec = 0;

    int result = select(maxfd+1, &read_set, NULL, NULL, &wait_time);
    if(result == -1){
      printf("No data yet\n");
    }
    else{
      for(int i=0; i<MAXFILES; i++){
        if(FD_ISSET(fds[i], &read_set)){
          int n = read(fds[i], buf, 1024);
          buf[n] = '\0';
          printf("%s has %d bytes of data: ",filenames[i],n);
          printf("|%s|\n",buf);
        }
      }
    }
  }


  for(int i=0; i<MAXFILES; i++){
    close(fds[i]);
  }
  exit(0);
}

