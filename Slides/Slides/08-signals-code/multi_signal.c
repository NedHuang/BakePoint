// multi_signal.c: adapted from Stevens/Rago figure 10.2, demonstrate
// use of signal() to set up a handler for SIGUSR1 and SIGUSR2. Show
// use with pause(). Typical usage is
//
// > gcc mult_signal.c
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
#include <stdlib.h>

int signal_received = 0;
// global variable indicates whether a signal was received.

// Handle multiple different signals in a single handler. Note that
// the signal handler is NOT reset in the function but still
// works. This is the behavior of most modern implementations.

void handle_usr_signals(int signo) {            // argument is signal number 
  signal_received = 1;
  if (signo == SIGUSR1){                        // check for signal type 1
    printf("received SIGUSR1\n");
  }
  else if (signo == SIGUSR2){                   // check for signal type 2
    printf("received SIGUSR2\n");
  }
  else{                                         // this should not be reachable
    printf("signo %d: Wait, how did I get here?\n",signo);
    exit(1);
  }
}

// Required to check the result of using the signal() function
typedef void (*sighandler_t)(int);

// main() function: set up signal handlers then sleep for an
// interval. Can be woken up by some signals.
//
// Note also default disposition for signals like CONT which are
// ignored while the program is running.
int main(void) {
  // Set signal handler and check result
  sighandler_t result = signal(SIGUSR1, handle_usr_signals);
  if(result == SIG_ERR){
    perror("couldn't set handler for SIGUSR1");
    exit(1);
  }

  // Short hand for set/check result
  if (signal(SIGUSR2, handle_usr_signals) == SIG_ERR){
    perror("couldn't set handler for SIGUSR2");
    exit(1);
  }

  while(1){
    sleep(4);                    // Sleep until a woken by a signal or 4 seconds passes
    if(signal_received == 1){
      printf("Signalled! Back to sleep now\n");
      signal_received = 0;
    }
    else{
      printf("Woke up after sleeping. Back to bed\n");
    }
  }

}

