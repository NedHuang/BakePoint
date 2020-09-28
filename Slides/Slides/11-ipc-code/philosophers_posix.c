// Dining philosophers (swansons) using Posix Semaphores
// Original: http://www.lisha.ufsc.br/~guto/teaching/os/exercise/phil.html
// Modified by Chris Kauffman
//
// Short random delays are added between each philosophers
// thinking/eating cycle to generate some variance in execution order
//
// To see the multiple processes, run the following commands
// 
// > gcc -o philosophers philosophers_posix.c -lpthread
// > philosophers > xxx & watch -d -n 0.1 'ps ux | grep philosophers'
// 
// Ctrl-c to stop the "watch" command (may screw up the terminal display)

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

#define N_PHILOSOPHERS       5     // Number of philosophers
const int MEALS_TO_FINISH = 10;    // Number of iterations before philosophers finish
const int MAX_DELAY = 50000;       // Maximum delay in nanos between meal iterations


sem_t *utensils[N_PHILOSOPHERS];   // semaphore pointers


int philosopher(int n);            // Defined after main()

int main(){
  setvbuf(stdout, NULL, _IONBF, 0); 
  pid_t phil[N_PHILOSOPHERS];
  printf("The Dining Swansons (Philosophers) Problem\n");

  // Parent process only:
  // 
  // Allocate utensils: semaphores which are initially set to value
  // one. 5 utensils total in an array.
  for(int i=0; i<N_PHILOSOPHERS; i++){
    char sem_name[256];
    sprintf(sem_name,"/utensil_%d",i);              // name starts with '/' and no other slashes
    utensils[i] =                                   // open named semaphore
      sem_open(sem_name, O_CREAT, S_IRUSR | S_IWUSR);
    int pshared = 1;                                // flag to indicate shared between processes
    sem_init(utensils[i], pshared, 1);              // initialize to value
  }

  // Parent generates child processes
  for(int i=0; i < N_PHILOSOPHERS; i++){
    int pid = fork();
    if(pid == 0){                                   // child has pid 0
      philosopher(i);                               // child acts as philosopher
    }
    else{                                           // parent gets pid > 0
      phil[i] = pid;                                // parent tracks children
    }
  }

  // Parent waits on all children to finish
  for(int i = 0; i < N_PHILOSOPHERS; i++) {
    waitpid(phil[i], NULL, 0);
  }

  printf("pausing prior to cleanup/exit (press enter to continue)\n");
  printf("while you're waiting, have a look in /dev/shm\n");
  getc(stdin);

  // Eliminate the utensils
  for(int i=0; i<N_PHILOSOPHERS; i++){
    // sem_close(utensils[i]);                         // close semaphore
    sem_wait(utensils[0]);

    char sem_name[256];                             // unlink the semaphore to remove from OS
    sprintf(sem_name,"/utensil_%d",i);              // without unlinking, will remain available 
    // sem_unlink(sem_name);                           // after process exits
  }

  return 0;
}

// Code for dining philosopher child processes 
int philosopher(int n){
  srand((n+1)*time(NULL));               // Seed random number generator


  // first utensil to get, most go for n first then n+1
  int first_utensil  = n;                // "left" utensil
  int second_utensil = n+1;              // "right" utensil

  // avoid deadlock via slightly different order of utensil requests
  // for last philospher: 0 first then n
  if(n == N_PHILOSOPHERS-1){
    first_utensil  = 0;                  // "right" utensil
    second_utensil = n;                  // "left" utensil
  }

  printf("Swanson %d: wants utensils %d and %d\n",n,first_utensil,second_utensil);

  int i;
  // Main loop of thinking/eating cycles
  for(i = 0; i < MEALS_TO_FINISH; i++) {
    int sleep_time = rand() % MAX_DELAY;
    usleep(sleep_time);         // sleep for for a short time
    printf("Swanson %d (egg %2d/%2d): contemplated his awesomeness for %d microseconds\n",
           n,i,MEALS_TO_FINISH,sleep_time);

    // get first utensil
    sem_wait( utensils[first_utensil] );
    printf("Swanson %d (egg %2d/%2d): got utensil %d\n", n,i,MEALS_TO_FINISH,first_utensil);

    // get second utensil
    sem_wait( utensils[second_utensil] );
    printf("Swanson %d (egg %2d/%2d): got utensil %d\n",n,i,MEALS_TO_FINISH,second_utensil);

    printf("Swanson %d (egg %2d/%2d): eating egg %d\n",n,i,MEALS_TO_FINISH,i);

    // release both utensils
    sem_post( utensils[first_utensil] );
    sem_post( utensils[second_utensil] );

    printf("Swanson %d (egg %2d/%2d): released utensil %d and utensil %d\n",
           n,i,MEALS_TO_FINISH,first_utensil,second_utensil);
  }

  printf("Swanson %d (egg %2d/%2d): leaving the diner\n",n,i,MEALS_TO_FINISH);
  exit(n);
}
