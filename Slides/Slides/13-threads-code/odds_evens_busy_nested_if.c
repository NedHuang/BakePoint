// odd/even thread coordination on incrementing an global
// variable. This version involves busy waiting on the odd/even
// condition by checking odd/even first, then locking, then
// re-checking.
#include "odds_evens.h"

int count = 0;                                       // global variable all threads are modifiying
pthread_mutex_t count_mutex;                         // mutex to check count

// Run by even child threads, increment count when it is even
void *even_work(void *t) {
  int tid = *( (int *) t);
  for(int i=0; i<THREAD_ITERS; ){ // no increment here
    if(count % 2 == 0){
      pthread_mutex_lock(&count_mutex);
      if(count % 2 == 0){                            // check for evenness
        printf("%d iter %d: count %d, IS EVEN, proceeding\n", tid, i, count);
        update(&count);                                     // now have lock and condition is correct can safely increment
        i++;
      }
      else{
        VPRINTF("%d iter %d: count %d, LOCKED NOT EVEN\n", tid, i, count);
      }
      pthread_mutex_unlock(&count_mutex);            // release lock allowing others to look/change count
    }
    else{
      VPRINTF("%d iter %d: count %d, NOT LOCKED NOT EVEN\n", tid, i, count);
    }      
  }
  printf("%d FINISHED %d iterations\n", tid, THREAD_ITERS);
  return NULL;
}

// Run by odd child threads, increment count when it is odd
void *odd_work(void *t) {
  int tid = *( (int *) t);
  for(int i=0; i<THREAD_ITERS; ){ // no increment here
    if(count % 2 == 1){
      pthread_mutex_lock(&count_mutex);
      if(count % 2 == 1){                            // check for evenness
        printf("%d iter %d: count %d, IS ODD, proceeding\n", tid, i, count);
        update(&count);                                     // now have lock and condition is correct can safely increment
        i++;
      }
      else{
        VPRINTF("%d iter %d: count %d, LOCKED NOT ODD\n", tid, i, count);
      }
      pthread_mutex_unlock(&count_mutex);            // release lock allowing others to look/change count
    }
    else{
      VPRINTF("%d iter %d: count %d, NOT LOCKED NOT ODD\n", tid, i, count);
    }      
  }
  printf("%d FINISHED %d iterations\n", tid, THREAD_ITERS);
  return NULL;
}


int main(int argc, char *argv[]) {
  pthread_t threads[TOT_THREADS];
  int tids[TOT_THREADS];

  pthread_mutex_init(&count_mutex, NULL);            // Initialize mutex  

  for(int i=0; i<TOT_THREADS; i+=2){
    tids[i] = i;
    pthread_create(&threads[i],   NULL, even_work, (void *) &(tids[i]));
    tids[i+1] = i+1;
    pthread_create(&threads[i+1], NULL, odd_work,  (void *) &(tids[i+1]));
  }

  for(int i=0; i<TOT_THREADS; i++) {                 // Wait for all threads to complete 
    pthread_join(threads[i], NULL);
  }

  printf("main: count is %d\n",count);

  pthread_mutex_destroy(&count_mutex);               // Clean up and exit
  exit(0);
}
