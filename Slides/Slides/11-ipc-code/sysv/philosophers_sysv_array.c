// Original version of dining philosphers using SysV
// semaphores. Rather that using a single semaphore of size 5 to
// control the chopsticks, this version uses a C array of 5 singular
// semaphores to control chopstick access
// 
// Source: http://www.lisha.ufsc.br/~guto/teaching/os/exercise/phil.html

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

const int ITERATIONS = 50;

int chopstick[5];

int philosopher(int n)
{
  int i, j, first, second;
  struct sembuf op;
  op.sem_num = 0;
  op.sem_flg = 0;

  printf("Philosopher %d was born!\n", n);
  srand(n);

  first = (n < 4)? n : 0; /* left for phil 0 .. 3, right for phil 4 */
  second = (n < 4)? n + 1 : 4; /* right for phil 0 .. 3, left for phil 4 */

  for(i = 0; i < ITERATIONS; i++) {
    printf("%2d: Philosopher %d is thinking ...\n", i,n);
    /* get first chopstick */
    op.sem_op = -1;
    semop(chopstick[first], &op, 1);
    /* get second chopstick */
    op.sem_op = -1;
    semop(chopstick[second], &op, 1);

    printf("%2d: Philosopher %d is eating ...\n", i,n);
    /* release first chopstick */
    op.sem_op = +1;
    semop(chopstick[first], &op, 1);
    /* release second chopstick */
    op.sem_op = +1;
    semop(chopstick[second], &op, 1);
    int sleep_time = rand() % 10;
    usleep(sleep_time);         // sleep for 0-9 microseconds
  }

  exit(n);
}

int main(){
  int i, status;
  pid_t phil[5];

  printf("The Dining-Philosophers Problem\n");

  // Parent process only:
  // 
  // Allocate chopsticks: semaphores which are initially set to value
  // 1. 5 chopsticks total in an array.
  for(i = 0; i < 5; i++) {
    if((chopstick[i] = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600)) < 0)
      return -1;
    if(semctl(chopstick[i], 0, SETVAL, 1) < 0)
      return -1;
  }

  // Parent generates child processes
  for(i = 0; i < 5; i++){
    int pid = fork();
    if(pid == 0){               // child has pid 0
      int ret = philosopher(i); // child acts as philosopher
      exit(ret);                // then exits
    }
    else{                       // parent gets pid > 0
      phil[i] = pid;            // parent tracks children
    }
  }

  // Parent waits on all children to finish
  for(i = 0; i < 5; i++) {
    waitpid(phil[i], &status, 0);
    if(WEXITSTATUS(status) == i)
      printf("Philosopher %d went to heaven!\n", i);
    else
      printf("Philosopher %d went to hell!\n", i);
  }

  // Eliminate the chopsticks semaphores
  for(i = 0; i < 5; i++)
    semctl(chopstick[i], 0, IPC_RMID, 0);

  return 0;
}
