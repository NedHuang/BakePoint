// append_os.c: uses open() flag O_APPEND which causes all writes to
// append, coordinated by the OS, no locking needed
//
// # COMPILE
// > gcc -g -o append_os append_os.c
// 
// # RUN IN A LOOP
// rm the_file.txt
// for i in $(seq 1000); do append_os $i & done 

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
  int fd = open(filename, O_CREAT | O_RDWR | O_APPEND, // last options guarantees appends
                S_IRUSR | S_IWUSR);
  //  lseek(fd, 0, SEEK_END);                          // no seek needed

  int n = strlen(word);
  word[n] = '\n';
  write(fd, word,n+1);
  close(fd);
  return 0;
}
