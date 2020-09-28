// faster_childe.c: demonstrate multiple children with wait(). After
// starting multiple child processes, uses a loop to wait for each to
// finish using the return value of wait() to determine which child
// finished.
// MAKE SURE to compile 'sleep_print' and 'complain' first

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv){

  char *children[][5] = {
    {"./sleep_print","1","wait for me",NULL},
    {"./complain",NULL},    
    {"ls","-l","-d",NULL},
  };


  for(int i=0; i<3; i++){
    pid_t child_pid = fork();
    if(child_pid == 0){
      execvp(children[i][0],children[i]);
      perror("Failed to execvp()");
    }
    else{
      printf("PARENT: Started '%s' in child proc %d\n",
             children[i][0], child_pid);
    }
  }
  // parent waits for each child
  for(int i=0; i<3; i++){
    int status;
    int child_pid = wait(&status);
    if(WIFEXITED(status)){
      int retval = WEXITSTATUS(status);
      printf("PARENT: Finished child proc %d, retval: %d\n",
             child_pid, retval);
    }
  }
  return 0;
}
  
