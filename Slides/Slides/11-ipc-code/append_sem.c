// append_sem.c: uses a semaphore to coorindate access to append to
// the file.
//
// # COMPILE
// > gcc -g -o append_sem append_sem.c -lpthread
// 
// # RUN IN A LOOP
// rm the_file.txt
// for i in $(seq 1000); do append_sem $i & done 


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
  char *sem_name = "/the_lock";
  char *filename = "the_file.txt";

  int fd = open(filename,
                O_CREAT | O_RDWR ,
                S_IRUSR | S_IWUSR);
  sem_t *sem = sem_open(sem_name,
                        O_CREAT,
                        S_IRUSR | S_IWUSR);
  sem_wait(sem);                // lock semaphore
  lseek(fd, 0, SEEK_END);

  int n = strlen(word);
  word[n] = '\n';
  write(fd, word,n+1);
  close(fd);

  sem_post(sem);                // unlock semaphore
  sem_close(sem);               // no longer need sempahore

  return 0;
}
