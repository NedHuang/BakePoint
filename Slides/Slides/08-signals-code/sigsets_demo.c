// sigsets_demo.c: simple demo of the sigset_t structure that is used
// struct sigaction and sigaction() function.
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>

int main(void) {
  sigset_t set;
  sigemptyset( &set );
  sigaddset( &set, SIGINT);
  sigaddset( &set, SIGTERM);
  sigaddset( &set, SIGUSR1);
  
  if( sigismember( &set, SIGINT ) ){
    printf("This set contains SIGINT\n");
  }

  sigdelset( &set, SIGUSR1);
  if( sigismember( &set, SIGUSR1 ) ){
    printf("This set contains SIGUSR1\n");
  }
  exit(0);
}
