// div0.c: demonstrates a failed attempt to handle SIGFPE which
// results from dividing by 0. Unfortunately, the signal is delivered
// and the same division instruction is restarted after the
// handler. Without a way to change the divisor, this will loop
// infinitely.
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Function run when a SIGINT is sent to the program
void handler(int sig_num) {
  printf("Divide by 0? Trying again!\n");
  fflush(stdout);
}
 
int main () {
  struct sigaction my_sa = {.sa_handler = handler};
  sigaction(SIGFPE, &my_sa, NULL); // catch div-0 signals
  int x=1, y=0;
  int result = x / y;
  printf("The mystery number is: %d\n",result);
  return 0;
}

  
