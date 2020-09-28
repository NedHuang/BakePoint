// Demonstrate use of mmap() and fstat() to transform a character in a
// file to a different character both specified on the command
// line. No reads()/writes() are done as mmap()'d files write back
// changed data automatically.
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
  if(argc < 4){
    printf("usage: %s <file> <in-char> <out-char>\n",argv[0]);
    printf("  Transforms all in-char to out-char in file\n");
    return 0;
  }
  char *filename = argv[1];                  // filename to operate on
  char in_char  = argv[2][0];                // in character to match
  char out_char = argv[3][0];                // out char to transform

  printf("Transforming '%c' to '%c' in %s\n",
         in_char, out_char, filename);

  int fd = open(filename, O_RDWR);           // open file to get file descriptor

  struct stat stat_buf;
  fstat(fd, &stat_buf);                      // get stats on the open file such as size
  int size = stat_buf.st_size;               // size for mmap()'ed memory is size of file


  char *file_chars =                         // pointer to file contents
    mmap(NULL, size, PROT_READ | PROT_WRITE, // call mmap with given size and file descriptor
         MAP_SHARED, fd, 0);                 // read/write, potentially share, offset 0

  for(int i=0; i<size; i++){                 // iterate through characters in file/mapped mem
    if(file_chars[i] == in_char){            // change in_char to out_char if found
      file_chars[i] = out_char;
    }
  }

  munmap(file_chars, size);                  // unmap and close file
  close(fd);
  printf("Transformation complete\n");

  return 0;
}
