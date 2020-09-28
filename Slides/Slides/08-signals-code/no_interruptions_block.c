// no_interruptions_block.c: demonstrate setting up a set of signals
// and blocking all of them.  This causes signals to not be
// delievered. After 5 seconds the default signal disposition is
// restored and any pending signals are delivered to the process.
//
// To stop this program from running, open another terminal and try
//  > pkill -9 a.out
// assuming you named the output program a.out
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum){
  printf("No interruptions\n");
}

int main () {
  sigset_t block_all, defaults;
  sigfillset( &block_all );                         // set contains all signals
  sigprocmask(SIG_SETMASK, &block_all, &defaults);  // set the mask to all signals
                                                    // and save defaults for later

  struct sigaction sa = {.sa_handler = handler };
  sigaction(SIGINT, &sa, NULL);                     // when unblocked, handle interrupts

  // infinite loop
  int count = 0;
  while(1) {        
    sleep(1);
    printf("Ma-na na-na!\n");
    fflush(stdout);
    count++;
    if(count == 5){
      printf("Restoring default signal disposition\n");
      sigprocmask(SIG_SETMASK, &defaults, NULL);    // restore defaults, pending signals delivered
    }
  }
  return 0;
}
