// no_interruptions_ignore.c: Use sigaction() to initially ignore
// SIGINT and SIGTERM but then restore the default behavior to
// terminate the program after a bit.
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main () {
  // Set handling functions for programs
  struct sigaction my_sa = {};               // portable signal handling setup with sigaction()
  sigemptyset(&my_sa.sa_mask);               // don't block any other signals during handling
  my_sa.sa_flags = SA_RESTART;               // always restart system calls on signals possible 

  my_sa.sa_handler = SIG_IGN;                // handle signals by ignoring them
  sigaction(SIGTERM, &my_sa, NULL);          // register SIGTERM with given action
  sigaction(SIGINT,  &my_sa, NULL);          // register SIGINT with given action
 
  // Infinite loop 
  int count = 0;
  while(1) {        
    count++;
    sleep(1);
    printf("Ma-na na-na!\n");
    fflush(stdout);

    if(count == 5){
      my_sa.sa_handler = SIG_DFL;            // change signal disposition back to default
      sigaction(SIGTERM, &my_sa, NULL);      // register SIGTERM with given action
      sigaction(SIGINT,  &my_sa, NULL);      // register SIGINT with given action
      printf("Do-doo-do-doo-do\n");
    }
  }
  return 0;
}
