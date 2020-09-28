// Writes data to a file which is readable and writable by the user by
// specificying permissions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 128

int main(int argc, char *argv[]){
  char *outfile = "readable.txt";

  printf("Opening file %s\n",outfile);
  mode_t mode = S_IRUSR | S_IWUSR;
  int out_fd = open(outfile,O_WRONLY | O_CREAT, mode);  // open for writing and create if needed, WITH PERMISSIONS
  if(out_fd == -1){                                     // check result and fail if not opened
    perror("Couldn't open output file");
    exit(1);
  }

  printf("Writing to file %s\n",outfile);
  char outbuf[BUFSIZE] = "here is some text to write";  // what to write
  int bytes_written = write(out_fd, outbuf, strlen(outbuf));   // do th write
  if(bytes_written == -1){                              // check for errors
    perror("Failed to read from file");
    exit(1);
  }
  printf("Wrote %d bytes to %s\n",bytes_written,outfile);

  if(close(out_fd) == -1){                              // check for errors on close
    perror("Couldn't close file");
  }

  return 0;
}
