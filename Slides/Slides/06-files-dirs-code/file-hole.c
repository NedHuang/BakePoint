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
  char buf1[] = "abcdefghij";
  char buf2[] = "ABCDEFGHIJ";

  int fd = open("holey.txt", flags,perms);
  write(fd, buf1, strlen(buf1));     // offset now = 10
  lseek(fd,  20, SEEK_CUR);          // offset now = 10+20
  write(fd, buf1, strlen(buf1));     // write data from 30-39
  lseek(fd, 50, SEEK_SET);           // offset now = 50
  write(fd, buf2, strlen(buf2));     // write from 50-59
  close(fd);
  exit(0);
}

