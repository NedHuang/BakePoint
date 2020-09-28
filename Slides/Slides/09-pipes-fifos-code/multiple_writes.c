// multiple_writes.c: Demonstrate 4 scenarios for writing to Files
// versus FIFOs to demonstrate subtle differences of OS position
// tracking in each.
//
// > a.out prefork file tmpt.txt 20
// 
// 1. Process opens normal file, forks, Parent / Child write. File position
// in file is shared.
//
// > a.out postfork file tmpt.txt 20
// 
// 2. Process forks, opens file, Parent / Child write. File position
// is NOT shared so will overwrite each other in file.
//
// > a.out prefork fifo tmpt.fifo 20 &
// > cat tmp.fifo
// 
// 3. Process opens a FIFO, forks, Parent / Child write. Both hang
// until something reads the pipe but all data is present.
// 
// > a.out postfork fifo tmpt.fifo 20 &
// > cat tmp.fifo
//
// 4. Proccess forks, opens FIFO, Parent / Child write. Both hang
// until something reads the pipe but all data is present.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 1024

int main(int argc, char *argv[]){
  if(argc < 5){
    printf("usage: %s {prefork,postfork} {file,fifo} <file> <iters>\n",argv[0]);
    exit(1);
  }

  char *open_prefork = argv[1];                                // open file before or after fork
  char *use_fifo = argv[2];                                    // whether to create a fifo or a normal file
  char *fname = argv[3];                                       // name of file to write
  int iters = atoi(argv[4]);                                   // number of iterations to write
  char msg[MAXSIZE];                                           // buffer for message
  int fd;
  
  remove(fname);                                               // get rid of file for creation
  if(strcmp(use_fifo,"fifo")==0){
    mkfifo(fname, S_IRUSR | S_IWUSR);                          // create the fifo if needed
  }

  if(strcmp(open_prefork,"prefork")==0){                       // open pre-fork, processes share position
    fd = open(fname, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);   // create if needed
  }
  char *id = "PARENT";
  int pid = fork();                                            // child inherits file descriptor
  if(pid == 0){                                                // child changes message
    id = "CHILD";
  }
  if(strcmp(open_prefork,"postfork")==0){                      // Open post-fork, each process has its own position
    fd = open(fname, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);   // create if needed
  }

  for(int i=0; i<iters; i++){
    snprintf(msg,MAXSIZE,"%3d: %s\n",i,id);                    // create the message
    int len = strlen(msg);
    write(fd, msg, len);                                       // write to file
  }

  close(fd);
  return 0;
}
