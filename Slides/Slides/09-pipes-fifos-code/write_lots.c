// Demonstrate writing to a file with repeated writes. Often run in
// simultaneous processes as in
//
// $> a.out file.txt 10 'AAA' & a.out file.txt 10 'BBB' &
//
// which will lead to either all AAA or all BBB messages in
// file.txt. Alternatively run with a fifo/named pipe as in
//
// $> mkfifo some.fifo
// $> a.out some.fifo 10 'AAA' & a.out some.fifo 10 'BBB' &
//
// Doing a cat some.fifo will complete both processes and show output
// intermingled from both.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 1024

int main(int argc, char *argv[]){
  if(argc < 4){
    printf("usage: %s file iters message\n",argv[0]);
    exit(1);
  }

  char *fname = argv[1];                                       // name of file to write
  int iters = atoi(argv[2]);                                   // number of iterations to write
  char *cmd_msg = argv[3];                                     // message to write each time
  char msg[MAXSIZE];                                           // buffer for message

  int fd = open(fname, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); // create if needed
  for(int i=0; i<iters; i++){
    snprintf(msg,MAXSIZE,"%3d: %s\n",i,cmd_msg);               // create the message
    int len = strlen(msg);
    int nbytes = write(fd, msg, len);                          // Write to file
    if(nbytes != len){                                         // Check if not enough has been written
      printf("only wrote %d bytes, wanted %d\n",nbytes,len);
      exit(1);
    }
  }

  close(fd);
  return 0;
}
