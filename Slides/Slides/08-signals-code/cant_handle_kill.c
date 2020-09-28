// cant_handle_kill.c: Demonstration that one cannot establish
// handlers or ignore SIGKILL.
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main () {
  struct sigaction my_sa = {};
  my_sa.sa_handler = SIG_IGN;
  int ret = sigaction(SIGKILL, &my_sa, NULL); // ignore SIGKILL?
  if(ret == -1){
    perror("failed to set up handler for SIGKILL");
    exit(1);
  }
 
  // Infinite loop 
  while(1) {        
    sleep(1);
    printf("Ma-na na-na!\n");
    fflush(stdout);
  }
  return 0;
}
