// Demonstrate use of mmap() and fstat() to print and increment all
// binary integers in a file named on the command line.
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s <file>\n",argv[0]);
    printf("  Print and Increment all binary ints in the given <file> \n");
    return 0;
  }
  char *filename = argv[1];         // filename to operate on
  int fd = open(argv[1], O_RDWR);   // open file descriptor

  struct stat stat_buf;
  fstat(fd, &stat_buf);           // get stats on the open file such as size
  int size = stat_buf.st_size;    // size for mmap()'ed memory is size of file


  int *file_ints = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  // get pointer to file bytes through mmap, treat as binary ints

  int len = size / sizeof(int);   // how many ints in file

  for(int i=0; i<len; i++){
    printf("%d\n",file_ints[i]);  // print all ints
  }

  for(int i=0; i<len; i++){
    file_ints[i] += 1;            // increment each file int, writes back to disk
  }

  munmap(file_ints, size);        // unmap and close file
  close(fd);
  printf("Transformation complete\n");

  return 0;
}
