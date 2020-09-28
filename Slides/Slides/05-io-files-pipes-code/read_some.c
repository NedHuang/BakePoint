// Basic demonstration of reading data from a file usin open(),
// read(), close() system calls.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 128

int main(int argc, char *argv[]){
  if(argc < 2){                                         // check command line for arguments
    printf("usage: %s <filename>\n",argv[0]);           // usage and exit if not given
    return 0;
  }

  char *infile = argv[1];                               // will read from file named on argv[1]

  printf("Opening %s\n",infile);
  int in_fd = open(infile,O_RDONLY);                    // open file for reading
  if(in_fd == -1){                                      // check result and fail if not opened
    perror("Couldn't open file");
    exit(1);
  }

  printf("Reading up to %d bytes from %s\n",BUFSIZE,infile);
  char buffer[BUFSIZE+1];                               // someplace to read into
  int bytes_read = read(in_fd, buffer, BUFSIZE);        // read from file, max BUFSIZE
  if(bytes_read == -1){                                 // check for errors
    perror("Failed to read from file");
    return 1;
  }

  buffer[bytes_read] = '\0';                            // null terminate string
  printf("Read %d chars which are:\n",bytes_read);      // print bytes read
  printf("====\n%s\n====\n", buffer);                   // print what was read

  int result = close(in_fd);
  if(result == -1){
    perror("Failed to close file");
    return 1;
  }

  return 0;
}
