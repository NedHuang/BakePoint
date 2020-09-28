// Initialize the semaphore used by append_sem.c

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  char *sem_name = "/the_lock";
  sem_t *sem = sem_open(sem_name, O_CREAT, S_IRUSR | S_IWUSR);
  sem_init(sem, 1, 1);
  sem_close(sem);
  return 0;
}
