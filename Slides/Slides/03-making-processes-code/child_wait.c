// child_wait.c: fork/exec plus parent waits for child to
// complete printing after each time.

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv){

  char *child_argv[] = {"ls","-l","-ah",NULL};          // argument array to child, must end with NULL 
  char *child_cmd = "ls";                               // actual command to run, must be on path

  // char *child_argv[] = {"./complain",NULL};          // alternative commands
  // char *child_cmd = "complain";

  printf("I'm %d, and I really don't feel like '%s'ing\n",
         getpid(),child_cmd);
  printf("I have a solution\n");

  pid_t child_pid = fork();

  if(child_pid == 0){
    printf("   I'm %d My pa '%d' wants me to '%s'. This sucks.\n",
           getpid(), getppid(), child_cmd);
    execvp(child_cmd, child_argv);
    printf("   I don't feel like myself anymore...\n"); // unreachable
  }
  else{
    int status;
    wait(&status);                                      // wait for child to finish, collect status
    printf("Great, junior %d is done with that '%s'ing\n",
           child_pid, child_cmd);
  }
  return 0;
}
  
