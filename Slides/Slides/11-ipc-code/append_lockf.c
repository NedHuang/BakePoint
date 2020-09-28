// append_lockf.c: uses a mandatory POSIX file lock to coorindate
// access to append to the file.
//
// # COMPILE
// > gcc -g -o append_lockf append_lockf.c
// 
// # RUN IN A LOOP
// rm the_file.txt
// for i in $(seq 1000); do append_lockf $i & done 

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

  lockf(fd, F_LOCK, 0);
  lseek(fd, 0, SEEK_END);

  int n = strlen(word);
  word[n] = '\n';
  write(fd, word,n+1);
  close(fd);

  lockf(fd, F_ULOCK, 0);
  return 0;
}
