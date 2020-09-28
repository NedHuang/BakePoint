// count_bytes.c: demonstrate how read can be used to count the bytes
// in a file. This is usually not needed as stat() calls will
// calculate this immediately.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 4

int main(int argc, char *argv[]){
  char *infile = argv[1];                       
  int in_fd = open(infile,O_RDONLY);
  char buf[BUFSIZE];
  int nread, total=0;
  while(1){
    nread = read(in_fd,buf,BUFSIZE-1);
    if(nread == 0){
      break;
    }
    buf[nread] = '\0';
    total += nread;
    printf("read: '%s'\n",buf);
  }
  printf("%d bytes total\n",total);
  close(in_fd);
  return 0;
}
