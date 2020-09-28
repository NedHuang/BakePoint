// Create a file with a gap. Examine contents of holey.txt to see a
// gap created by text.
// 
// Inspired by Stevens/Rago Fig 3.2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(void) {
  int flags = O_WRONLY | O_CREAT;
  mode_t perms = S_IRUSR | S_IWUSR;
  char buf1[] = "ZYXW";

  int fd = open("holey.txt", flags,perms);
  lseek(fd,  20, SEEK_SET);          // offset now = 20
  write(fd, buf1, strlen(buf1));     // write data from 30-39
  close(fd);
  exit(0);
}

