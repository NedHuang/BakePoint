// Dining philosophers (swansons) using System V Semaphores
// Original: http://www.lisha.ufsc.br/~guto/teaching/os/exercise/phil.html
// Modified by Chris Kauffman
//
// Short random delays are added between each philosophers
// thinking/eating cycle to generate some variance in execution order
//
// To see the multiple processes, run the following commands
// > gcc -o philosophers philosophers.c
// > philosophers > xxx & watch -d -n 0.1 'ps ux | grep philosophers'
// Ctrl-c to stop the "watch" command (may screw up the terminal display)

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

const int N_PHILOSOPHERS  =  5; // Number of philosophers
const int MEALS_TO_FINISH = 10; // Number of iterations before philosophers finish
const int MAX_DELAY = 50000;    // Maximum delay in nanos between meal iterations

// Semaphore ids
int utensils;                 // ID for array of IPC semaphores
int not_at_table;               // Start dinner when semaphore reaches 0

int philosopher(int n);         // Defined after main()

int main(){
  setvbuf(stdout, NULL, _IONBF, 0); 
  int i, status;
  pid_t phil[N_PHILOSOPHERS];
  printf("The Dining Swansons (Philosophers) Problem\n");

  // Parent process only:
  // 
  // Allocate utensils: semaphores which are initially set to value
  // one. 5 utensils total in an array. Private semaphore array for
  // parent/children.
  utensils = semget(IPC_PRIVATE, N_PHILOSOPHERS, IPC_CREAT | 0600); 
  for(i=0; i<N_PHILOSOPHERS; i++){
    semctl(utensils, i, SETVAL, 1);
  }
  not_at_table = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

  // Prevent children from starting to eat until all have checked
  // in. Each child decreases the not_at_table counter by one then
  // wait until it reaches 0 to start eating.
  semctl(not_at_table, 0, SETVAL, 5);

  // Parent generates child processes
  for(i=0; i < N_PHILOSOPHERS; i++){
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
  for(i = 0; i < N_PHILOSOPHERS; i++) {
    waitpid(phil[i], &status, 0);
  }

  // Eliminate the utensils and table semaphores
  semctl(utensils, 0, IPC_RMID, 0);
  semctl(not_at_table, 0, IPC_RMID, 0);
  return 0;
}

// Code for dining philosopher child processes 
int philosopher(int n){
  struct sembuf op;                      // Used to perform semaphore operations
  op.sem_flg = 0;
  srand((n+1)*time(NULL));               // Seed random number generator

  // Avoid deadlock via slightly different order of utensil requests
  // for last philospher

  // first utensil to get, most go for n first, last philospher goes for 0 first
  int first_utensil  = n;                // "left" utensil
  int second_utensil = n+1;              // "right" utensil

  // last philosopher changes order of acquiring utensils to avoid deadlock
  if(n == N_PHILOSOPHERS-1){
    first_utensil  = 0;                  // "right" utensil
    second_utensil = n;                  // "left" utensil
  }

  printf("Swanson %d: wants utensils %d and %d\n",n,first_utensil,second_utensil);

  // Check in for dinner by decrementing the not_at_table value.
  op.sem_op = -1;
  op.sem_num = 0;
  semop(not_at_table, &op, 1);
  printf("Swanson %d at the table\n", n);

  // Wait for everyone to check in before start the meal. Passing 0 as
  // the sem_op means wait until the not_at_table counter is 0 to
  // proceed.
  op.sem_op = 0;
  op.sem_num = 0;
  semop(not_at_table, &op, 1);

  int i;
  // Main loop of thinking/eating cycles
  for(i = 0; i < MEALS_TO_FINISH; i++) {
    int sleep_time = rand() % MAX_DELAY;
    usleep(sleep_time);         // sleep for for a short time
    printf("Swanson %d (egg %2d/%2d): contemplated his awesomeness for %d microseconds\n",
           n,i,MEALS_TO_FINISH,sleep_time);

    // get first utensil
    op.sem_op = -1;
    op.sem_num = first_utensil;
    semop(utensils, &op, 1);
    printf("Swanson %d (egg %2d/%2d): got utensil %d\n", n,i,MEALS_TO_FINISH,first_utensil);

    // get second utensil
    op.sem_op = -1;
    op.sem_num = second_utensil;
    semop(utensils, &op, 1);
    printf("Swanson %d (egg %2d/%2d): got utensil %d\n",n,i,MEALS_TO_FINISH,second_utensil);

    printf("Swanson %d (egg %2d/%2d): eating egg %d\n",n,i,MEALS_TO_FINISH,i);

    // release both utensils
    struct sembuf rel_op[2];             // Used to perform semaphore operations
    rel_op[0].sem_flg = 0;
    rel_op[0].sem_op = +1;
    rel_op[0].sem_num = first_utensil;
    rel_op[1].sem_flg = 0;
    rel_op[1].sem_op = +1;
    rel_op[1].sem_num = second_utensil;
    semop(utensils, rel_op, 2);          // 2 operations

    printf("Swanson %d (egg %2d/%2d): released utensil %d and utensil %d\n",
           n,i,MEALS_TO_FINISH,first_utensil,second_utensil);
  }

  printf("Swanson %d (egg %2d/%2d): leaving the diner\n",n,i,MEALS_TO_FINISH);
  exit(n);
}
