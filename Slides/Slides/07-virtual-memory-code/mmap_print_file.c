// Demonstrate use of mmap() and fstat() to print all characters in a
// file without use of read()
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s <file>\n",argv[0]);
    return 0;
  }

  int fd = open(argv[1], O_RDONLY);          // open file to get file descriptor

  struct stat stat_buf;
  fstat(fd, &stat_buf);                      // get stats on the open file such as size


  int size = stat_buf.st_size;               // size for mmap()'ed memory is size of file
  char *file_chars =                         // pointer to file contents
    mmap(NULL, size, PROT_READ, MAP_SHARED,  // call mmap with given size and file descriptor
         fd, 0);                             // read only, potentially share, offset 0

  for(int i=0; i<size; i++){                 // iterate through characters in file/mapped mem
    printf("%c",file_chars[i]);              // print each
  }
  printf("\n");

  munmap(file_chars, size);                  // unmap and close file
  close(fd);
  return 0;
}
