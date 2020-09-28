// A C program that does not terminate from an interrupt signal.
// Usually pressing Ctrl-C sends this to the foreground program.  This
// version ignores signals silently for a while, then restores the
// default signal handlers so the program can be terminated normally.
//
// To stop this program from running, open another terminal and try
//  > pkill -9 a.out
// assuming you named the output program a.out
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main () {
  // Set handling functions for programs
  signal(SIGINT, SIG_IGN);      // Ignore these signals silently
  signal(SIGTERM, SIG_IGN);
 
  // Infinite loop
  int count = 0;
  while(1) {        
    sleep(1);
    printf("Ma-na na-na!\n");
    fflush(stdout);
    count++;
    if(count == 5){
      printf("Restoring default signal disposition\n");
      signal(SIGINT, SIG_DFL);  // restore default signal disposition
      signal(SIGTERM, SIG_DFL);
    }
  }
  return 0;
}
