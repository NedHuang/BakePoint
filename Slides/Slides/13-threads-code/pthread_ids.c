// Small example showing non-portable printing of thread IDs which are
// often just unsigned long
#include <pthread.h>
#include <stdio.h>

void *doit(void *param){
  pthread_t tid = pthread_self();
  printf("doit: I am thread %ld\n",tid);
  int *var = (int *) param;
  *var = (*var) * 2;
  return NULL;
}

int main(){
  pthread_t thread_1;
  int stackvar = 42;
  pthread_create(&thread_1, NULL, doit, &stackvar);
  pthread_t tid = pthread_self();
  printf("main: I am thread %ld\n",tid);
  pthread_join(thread_1, NULL);
  printf("stack var is: %d\n",stackvar);
  return 0;
}
