// Minimal example of starting a pthread, passing a parameter to the
// thread function, then waiting for it to finish
#include <pthread.h>
#include <stdio.h>

void *doit(void *param){
  int p=(int) param;
  p = p*2;
  return (void *) p;
}

int main(){
  pthread_t thread_1;
  pthread_create(&thread_1, NULL, doit, (void *) 42);
  int xres;
  pthread_join(thread_1, (void **) &xres); 
  printf("result is: %d\n",xres);
  return 0;
}
