// Working version of threaded picalc. Uses a mutex to lock the global
// variable total_hits so that one thread accesses it at a time. This
// version is inefficient due to the busy waiting and contention
// associated with mutex locking.

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

// Global variables shared by all threads
int total_hits=0;
int points_per_thread = -1;
pthread_mutex_t lock;

void *compute_pi(void *arg){
  long thread_id = (long) arg;
  unsigned int rstate = 123456789 * thread_id;   // give each thread its own starting point
  for (int i = 0; i < points_per_thread; i++) { 
    double x = ((double) rand_r(&rstate)) / ((double) RAND_MAX);
    double y = ((double) rand_r(&rstate)) / ((double) RAND_MAX);
    if (x*x + y*y <= 1.0){
      pthread_mutex_lock(&lock);                 // lock global variable
      total_hits++;                              // update
      pthread_mutex_unlock(&lock);               // unlock global variable
    }
  } 
  return NULL;
}

int main(int argc, char **argv) { 
  if(argc < 2){
    printf("usage: %s <num_samples> [num_threads]\n",argv[0]);
    printf("  num_samples: int, how many sample points to try, higher gets closer to pi\n");
    printf("  num_threads: int, number of threads to use for the computation, default 4\n");
    return -1;
  }

  int npoints = atoi(argv[1]);                   // number of samples
  int num_threads = argc>2 ? atoi(argv[2]) : 4;  // number of threads

  points_per_thread = npoints / num_threads;     // init global variables
  pthread_mutex_init(&lock, NULL);               // create a lock

  pthread_t threads[num_threads];                // track each thread

  for(long p=0; p<num_threads; p++){             // launch each thread
    pthread_create(&threads[p],NULL,compute_pi, (void *) (p+1));
  }

  for(int p=0; p<num_threads; p++){              // wait for each thread to finish
    pthread_join(threads[p], (void **) NULL);
  }

  pthread_mutex_destroy(&lock);                  // get rid of the lock
  
  double pi_est = ((double)total_hits) / npoints * 4.0;
  printf("npoints: %8d\n",npoints);
  printf("hits:    %8d\n",total_hits);
  printf("pi_est:  %f\n",pi_est);
}
