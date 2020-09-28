// start_stop.c: Demonstrate use of SIGSTOP and SIGCONT to stop and
// restart a child process.

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
  if(argc < 3){
    printf("usage: %s <program> <int>\n",argv[0]);
    exit(0);
  }
  char *child_prog = argv[1];                // program to run as a child
  int interval = atoi(argv[2]);              // time to allow child to run

  int pid = fork();                          // fork execution

  if(pid == 0){                              // CHILD
    execlp(child_prog, child_prog, NULL);    // execute specified program
  }

  int status, ret;                           // PARENT
  while(1){                                  // Loop until child done
    int result;
    sleep(interval);                         // interval to allow child to run
    result = kill(pid,SIGSTOP);              // send a stop signal to child
    printf("SIGSTOP result: %d\n",result);   // check on delivery
    sleep(interval);                         // interval to allow child to run
    result = kill(pid,SIGCONT);              // send a continue signal to child
    printf("SIGCONT result: %d\n",result);   // check on delivery

    ret = waitpid(pid, &status, WNOHANG);    // check on child
    if(ret == pid){                          // if child is finished
      break;                                 //   break from loop
    }
  }

  if(WIFSIGNALED(status)){                   // check if a signal ended the child
    printf("child process %d terminated with signal %d\n",
           pid,WTERMSIG(status));
  }

  exit(0);
}
