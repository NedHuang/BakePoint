// Use of fwrite() to create a file of binary integers
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
  if(argc < 3){
    printf("usage: %s <file> <length>\n",argv[0]);
    printf("  Create <file> with binary ints 0,1,2,... <length-1> in it\n");
    return 0;
  }
  char *filename = argv[1];         // filename to operate on
  FILE *file = fopen(argv[1], "w"); // open file handle
  int len  = atoi(argv[2]);

  for(int i=0; i<len; i++){
    fwrite(&i,sizeof(int),1,file); // write binary int to file
  }

  fclose(file);

  return 0;
}
