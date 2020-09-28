// Demonstrate umask: restrictions on permissions for freshly created
// files with open()/creat() files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){

  mode_t new_mask = 0000;                      // allow any permissions
  mode_t old_mask = umask(new_mask);           // set umask to 

  int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;;
  int flags = O_WRONLY | O_CREAT;
  int out_fd = -1;
  char *message = NULL;
  // Write to first file
  out_fd = open("readable1.txt",flags,perms ); // create file 
  message = "here is some text to write";      // what to write
  write(out_fd, message, strlen(message));     // do the write
  close(out_fd);                               // sloppy close

  // readable1.txt should have perms: -rw-rw----

  new_mask = 0077;                             // don't allow permissions to be set for group or other
  old_mask = umask(new_mask);

  // Write to first file
  out_fd = open("readable2.txt",flags,perms);  // open for writing and create if needed, PERMISSIONS
  message = "more text for writing";           // what to write
  write(out_fd, message, strlen(message));     // do the write
  close(out_fd);                               // sloppy close

  // readable1.txt should have perms: -rw-------
  // No group perms as umask stopped this

  return 0;
}
