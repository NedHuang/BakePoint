// protected_reentrant.c: protect non-reentrant function using signal
// blocking. This prevents stalls but NOT may yield incorrect results
// if the signals are unblocked before the CRITICAL section can be
// completed.
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
  sigset_t block_all, defaults;                       // two sets of signals
  sigfillset( &block_all );                           // contains all signals
  sigprocmask(SIG_SETMASK, &block_all, &defaults);    // block all signals
  struct passwd *rootptr = NULL;

  // NON-REENTRANT FUNCTION CALL IN SIGNAL HANDLER
  rootptr = getpwnam("root");                         // fetch record associated with root password

  if (rootptr == NULL){                               // check for presence of root
    printf("no root, that's weird...\n");
  }
  else{
    printf("root looks okay\n");
  }
  alarm(1);                                           // reset the alarm
  sigprocmask(SIG_SETMASK, &defaults, NULL);          // un-block signals
  printf("leaving signal handler\n");
}

int main(void) {
  signal(SIGALRM, alarm_handler);                     // handle alarms in with the above function
  alarm(1);

  int successes = 0;
  printf("Repeatedly checking on user '%s'\n",USERNAME);
  while(1){
    // Block signals while calling non-reentrant function
    sigset_t block_all, defaults;                     // two sets of signals
    sigfillset( &block_all );                         // contains all signals
    sigprocmask(SIG_SETMASK, &block_all, &defaults);  // block all signals
    struct passwd *ptr = getpwnam(USERNAME);          // non-reentrant call
    // sigprocmask(SIG_SETMASK, &defaults, NULL);        // un-block signals, incorrect position

    if(ptr == NULL){
      perror("couldn't find user");
      exit(1);
    }
    if(strcmp(ptr->pw_name, USERNAME) != 0){          // should always be equal to USERNAME
      printf("return value corrupted! pw_name = %s\n",
             ptr->pw_name);
      exit(1);
    }


    successes++;                                      // Show progress of loop
    if(successes % 100000 == 0){
      printf("%d successes so far\n",successes);
    }
  }
  exit(0);
}
