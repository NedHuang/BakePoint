// sig_usr.c: adopted from Stevens/Rago figure 10.2, demonstrate use
// of signal() to set up a handler for SIGUSR1 and SIGUSR2. Show use
// with pause(). Typical usage is
//
// > gcc sig_usr.c
// > a.out &
// [1] 28488
// > kill -SIGUSR1 %1
// received SIGUSR1
// > kill -SIGUSR2 %1
// received SIGUSR2
// > kill -SIGUSR1 %1
// received SIGUSR1
// > kill -TERM %1
// > 
// [1]+  Terminated              a.out

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// one handler for both signals */
void sig_usr(int signo) { // argument is signal number
  if (signo == SIGUSR1)
    printf("received SIGUSR1\n");
  else if (signo == SIGUSR2)
    printf("received SIGUSR2\n");
  else
    fprintf(stderr,"received signal %d\n", signo);
}

int main(void) {
  if (signal(SIGUSR1, sig_usr) == SIG_ERR){
    fprintf(stderr,"can’t catch SIGUSR1\n");
  }
  if (signal(SIGUSR2, sig_usr) == SIG_ERR){
    fprintf(stderr,"can’t catch SIGUSR2\n");
  }
  while(1){
    pause();
  }
}
