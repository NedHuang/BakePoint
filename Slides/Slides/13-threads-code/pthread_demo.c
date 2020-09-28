// Small example showing non-portable printing of thread IDs which are
// often just unsigned long
#include <pthread.h>
#include <stdio.h>

void *doit(void *param){
  long me = (long) param;
  printf("doit: I am thread %ld\n",me);
  return NULL;
}

int main(){
  pthread_t thread_1, thread_2;
  pthread_create(&thread_1, NULL, doit, 1);
  pthread_create(&thread_1, NULL, doit, 2);
  printf("main: I am thread %ld\n",tid);
  pthread_join(thread_1, NULL);
  return 0;
}
