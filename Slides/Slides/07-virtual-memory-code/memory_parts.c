// memory_parts.c: Allocate memory for variables in different regions
// then pause to allow command pmap to be run.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


char global_arr[4096] = {};
double global_var = 1.2345;

int main(int argc, char *argv[]){
  char local_arr[512] = {};

  char *malloc_arr = malloc(128);

  // memory map a file
  int fd = open("gettysburg.txt", O_RDONLY); // open file to get file descriptor
  struct stat stat_buf;
  fstat(fd, &stat_buf);                      // get stats on the open file such as size
  int size = stat_buf.st_size;               // size for mmap()'ed memory is size of file
  char *file_chars =                         // pointer to file contents
    mmap(NULL, size, PROT_READ, MAP_PRIVATE, // call mmap with given size and file descriptor
         fd, 0);                             // read only, not shared, offset 0

  printf("%p : main()\n", main);
  printf("%p : global_arr\n", global_arr);
  printf("%p : local_arr\n",  local_arr);
  printf("%p : malloc_arr\n", malloc_arr);
  printf("%p : mmap'd file\n", file_chars);
  int pid = getpid();
  printf("my pid is %d\n",pid);

  printf("press any key to continue\n");
  getc(stdin);                  // get a character

  munmap(file_chars, size);                  // unmap and close file
  close(fd);

  return 0;
}
