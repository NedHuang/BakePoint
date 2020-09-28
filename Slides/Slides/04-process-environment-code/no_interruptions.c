// no_interruptions.c: A C program that is not killed from interrupt
// or terminate signals by establishing handlers that ignore
// signals. sigaction() is used to set up the signal handlers.
//
// To stop this program from running, open another terminal and try
//  > pkill -9 a.out
// assuming you named the output program a.out
//
// This version uses a LOT of CPU as it is constantly incrementing an
// integer in a tight loop.
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Function run when a SIGINT is sent to the program
void handle_signals(int sig_num) {
  if(sig_num == SIGINT){
    printf("\nNo SIGINT-erruptions allowed.\n");
  }
  else if(sig_num == SIGTERM){
    printf("\nTry to SIGTERM me? Piss off!\n");
  }
  fflush(stdout);
}
 
int main () {
  // Set handling functions for programs
  struct sigaction my_sa = {};               // portable signal handling setup with sigaction()

  my_sa.sa_handler = handle_signals;         // run function handle_SIGTERM
  sigaction(SIGTERM, &my_sa, NULL);          // register SIGTERM with given action
  sigaction(SIGINT,  &my_sa, NULL);          // register SIGINT with given action
 
  for(int x=0; 1; x++){                      // Infinite loop 
    if(x==0){
      printf("Ma-na na-na!\n");
      fflush(stdout);
    }
  }
  return 0;
}
