// Demonstrates that pthread_mutex_lock() is not a busy wait. Two
// threads obtain a lock and then sleep for a second.  The total
// runtime should be about 2 seconds as only one thread can have the
// lock at a time. If lock waiting was busy, the user time should be
// about 1s as one thread busily waits for the lock. However user time
// is usually measured in the thousandths of seconds as threads sleep
// then wake up when the the lock is available.
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int glob = 1;
pthread_mutex_t glob_lock;

void *doit(void *param){
  pthread_mutex_lock(&glob_lock);
  glob = glob*2;
  sleep(1);
  pthread_mutex_unlock(&glob_lock);
  return NULL;
}

int main(){
  printf("BEFORE glob: %d\n",glob);

  pthread_mutex_init(&glob_lock, NULL);
  pthread_t thread_1;
  pthread_create(&thread_1, NULL, doit, NULL);
  pthread_t thread_2;
  pthread_create(&thread_2, NULL, doit, NULL);

  pthread_join(thread_1, (void **) NULL); 
  pthread_join(thread_2, (void **) NULL); 

  printf("AFTER glob: %d\n",glob);
  pthread_mutex_destroy(&glob_lock);
  
  return 0;
}
