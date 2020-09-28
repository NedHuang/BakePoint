// non_reentrant.c: Shows dangers of using a non-reentrant function
// with signal handling. Strange things can happen due to the use of
// the non-reentrant function getpwnam() being called both in the main
// loop and signal handler. Uncommenting the getpwnam() in the signal
// handler causes unpredictable things to happen like a permanent
// stall of the program on Linux boxes.
// 
// Adapted from Stevens and Rago Fig 10.5. 
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

#define USERNAME "kauffman"

static void alarm_handler(int signo) {
  printf("in signal handler\n");
  struct passwd *rootptr = NULL;

  // NON-REENTRANT FUNCTION CALL IN SIGNAL HANDLER
  rootptr = getpwnam("root");                    // fetch record associated with root password

  if (rootptr == NULL){                          // check for presence of root
    printf("no root, that's weird...\n");
  }
  else{
    printf("root user present\n");
  }
  alarm(1);                                      // reset the alarm
  printf("leaving signal handler\n");
}

int main(void) {
  struct sigaction my_sa = {
    .sa_handler = alarm_handler,
  };
  sigaction(SIGALRM, &my_sa, NULL);          // handle alarms in with the above function
  alarm(1);                                  // set an alarm to ring in 1 second

  int successes = 0;
  printf("Repeatedly checking on user '%s'\n",USERNAME);
  while(1){
    struct passwd *ptr = getpwnam(USERNAME);     // non-reentrant call
    if(ptr == NULL){
      perror("couldn't find user");
      exit(1);
    }
    if(strcmp(ptr->pw_name, USERNAME) != 0){     // should always be equal to USERNAME
      printf("return value corrupted!, pw_name = %s\n",
             ptr->pw_name);
      exit(1);
    }
    successes++;                                 // Show progress of loop
    if(successes % 100000 == 0){
      printf("%d successes so far\n",successes);
    }
  }
  exit(0);
}
