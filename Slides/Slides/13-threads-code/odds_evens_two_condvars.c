// odd/even thread coordination on incrementing an global
// variable. This version uses two condition variables to separately
// notify threads waiting on oddness and evenness. This further
// improves resource utilization.
#include "odds_evens.h"

int count = 0;                                       // global variable all threads are modifiying
pthread_mutex_t count_mutex;                         // mutex to check count
pthread_cond_t  even_condv;                          // receive notification count is even
pthread_cond_t  odd_condv;                           // receive notification count is odd

// Run by even child threads, increment count when it is even
void *even_work(void *t) {
  int tid = *( (int *) t);
  for(int i=0; i<THREAD_ITERS; i++){
    pthread_mutex_lock(&count_mutex);
    while(count % 2 != 0){                           // wait until count is even
      VPRINTF("%d iter %d: count %d, NOT EVEN, sleeping\n",
             tid, i, count);
      pthread_cond_wait(&even_condv, &count_mutex); // await notification to check again, gets lock on wakeup
    }
    printf("%d iter %d: count %d, IS EVEN, proceeding\n",
           tid, i, count);
    update(&count);                                         // now have lock and condition is correct can safely increment
    pthread_mutex_unlock(&count_mutex);              // release lock
    pthread_cond_signal(&odd_condv);
  }
  printf("%d FINISHED %d iterations\n", tid, THREAD_ITERS);
  return NULL;
}

// Run by odd child threads, increment count when it is odd
void *odd_work(void *t) {
  int tid = *( (int *) t);
  for(int i=0; i<THREAD_ITERS; i++){
    pthread_mutex_lock(&count_mutex);
    while(count % 2 != 1){                           // wait until count is odd
      VPRINTF("%d iter %d: count %d, NOT ODD, sleeping\n",
             tid, i, count);
      pthread_cond_wait(&odd_condv, &count_mutex); // await notification to check again, gets lock on wakeup
    }
    printf("%d iter %d: count %d, IS ODD, proceeding\n",
           tid, i, count);
    update(&count);                                         // now have lock can safely increment
    pthread_mutex_unlock(&count_mutex);              // release lock
    pthread_cond_signal(&even_condv);
  }
  printf("%d FINISHED %d iterations\n", tid, THREAD_ITERS);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t threads[TOT_THREADS];
  int tids[TOT_THREADS];

  pthread_mutex_init(&count_mutex, NULL);            // Initialize mutex and condition variable 
  pthread_cond_init (&even_condv,  NULL);
  pthread_cond_init (&odd_condv,   NULL);

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
  pthread_cond_destroy(&even_condv);
  pthread_cond_destroy(&odd_condv);
  exit(0);
}

