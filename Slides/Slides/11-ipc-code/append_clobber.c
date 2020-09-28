// append_clobber.c: write to the end of a file. No coordination
// mechanism is used so multiple processes writing the end of the file
// may clobber the data there.
//
// # COMPILE
// > gcc -g -o append_clobber append_clobber.c
// 
// # RUN IN A LOOP
// rm the_file.txt
// for i in $(seq 1000); do append_clobber $i & done 

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s <word>\n");
    return 1;
  }

  char *word = argv[1];
  char *filename = "the_file.txt";

  int fd = open(filename, O_CREAT | O_RDWR , S_IRUSR | S_IWUSR);
  lseek(fd, 0, SEEK_END);

  int n = strlen(word);
  word[n] = '\n';
  write(fd, word,n+1);
  close(fd);

  return 0;
}
