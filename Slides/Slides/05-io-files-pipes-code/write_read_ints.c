// write_read_ints.c: Demonstration of writing binary integer data to
// a file the reading it back.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define COUNT 16

int main(int argc, char *argv[]){
  char *outfile = "integers.dat";                       // make sure this file exists

  printf("Opening file %s\n",outfile);
  int out_fd = open(outfile,O_WRONLY);                  // open for writing, must exist 
  if(out_fd == -1){                                     // check result and fail if not opened
    perror("Couldn't open output file");
    exit(1);
  }

  printf("Filling output buffer\n");
  int out_ints[COUNT];
  for(int i=0; i<COUNT; i++){
    out_ints[i] = i;
  }

  int bufsize = sizeof(int)*COUNT;                      // calculate size of output
  printf("Writing up to %d bytes to file %s\n",
         bufsize,outfile);
  int bytes_written = write(out_fd, out_ints, bufsize);   // do th write
  if(bytes_written == -1){                              // check for errors
    perror("Failed to read from file");
    exit(1);
  }
  printf("Wrote %d bytes to %s\n",bytes_written,outfile);

  if(close(out_fd) == -1){                              // check for errors on close
    perror("Couldn't close file");
  }
  
  int in_fd = open(outfile,O_RDONLY);                   // open file for reading
  if(in_fd == -1){                                      // check result and fail if not opened
    perror("Couldn't open file");
    exit(1);
  }

  printf("Opening %s\n",outfile);

  printf("Reading up to %d bytes from %s\n",bufsize,outfile);
  int in_ints[COUNT];
  int bytes_read = read(in_fd, in_ints, bufsize);         // read from file, max BUFSIZE
  if(bytes_read == -1){                                 // check for errors
    perror("Failed to read from file");
    exit(1);
  }

  printf("Read %d bytes, as numbers:\n",bytes_read);      // print bytes read
  for(int i=0; i<COUNT; i++){
    printf("%d ",in_ints[i]);
  }
  printf("\n");

  int result = close(in_fd);
  if(result == -1){
    perror("Failed to close file");
    exit(1);
  }

  return 0;
}
