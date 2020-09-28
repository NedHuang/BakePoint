#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  2                               // number of odd/evens threads
#define TOT_THREADS (2*NUM_THREADS)                  // total threads

#ifndef THREAD_ITERS
#define THREAD_ITERS 5                               // total iterations  each thread will add on
#endif

#ifndef VPRINTF
#define VPRINTF printf
#endif
void ignore(char *s, int i, int j, int k){} // ignore requests to print

#ifndef SLEEPFUN 
#define SLEEPFUN usleep(1)
#endif

void update(int *x){
  *x = *x+1;
  SLEEPFUN;          // simulate some "work" done while locked
}
