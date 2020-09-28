// crypt_protected.c: Shows how to protect non-reentrant call to
// crypt() during a main loop by blocking signals during the critical
// section.
// 
// COMPILE with libcrypt
// > gcc crypt_not_reentrant.c -lcrypt    
// 
// Modeled after Stevens and Rago Fig 10.5 which uses getpwnam()
// instead of crypt().
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

#define PASSWORD "password123"
#define SALT     "00"
char reference[128];                             // contains encrypted version of password

void alarm_handler(int signo) {
  printf("in signal handler\n");
  char *crypted = crypt(PASSWORD,SALT);              // call non-reentrant function in handler
  if( strncmp(reference, crypted, 128) != 0 ){       // check whether this matches the reference
    printf("MISMATCH: %s\n",crypted);                // nope... wonder why not?
    exit(1);
  }
  printf("HANDLER ENCRYPTED: %s\n",crypted);
  printf("leaving signal handler\n");
  alarm(1);                                          // set alarm to ring in another second
}

int main(void) {
  struct sigaction my_sa = {
    .sa_handler = alarm_handler,
  };
  sigaction(SIGALRM, &my_sa, NULL);                  // handle alarms in with the above function
  alarm(1);                                          // set an alarm to ring in 1 second

  printf("Repeatedly crypting '%s'\n",PASSWORD);

  char *refcrypted = crypt(PASSWORD,SALT);           // generate the reference encrypted password
  strncpy(reference,refcrypted,128);
  printf("REFERENCE ENCRYPTED: %s\n",reference);


  int successes = 0;
  while(1){

    sigset_t block_all, defaults;                    // two sets of signals
    sigfillset( &block_all );                        // contains all signals
    sigprocmask(SIG_SETMASK, &block_all, &defaults); // block all signals

    char *crypted = crypt(PASSWORD,SALT);            // encrypt


    if( strncmp(reference, crypted, 128) != 0 ){     // check whether this matches the reference
      printf("MISMATCH: %s\n",crypted);              // should never happen now
      exit(1);
    }

    sigprocmask(SIG_SETMASK, &defaults, NULL);       // un-block signals, correct position

    successes++;                                     // show progress of loop
    if(successes % 10000 == 0){
      printf("%d successes so far\n",successes);
    }
  }
  return 0;
}
