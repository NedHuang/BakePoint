// impatient_parent.c: demonstrate non-blocking waitpid(), 

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv){
  char *child_argv[] = {"./complain",NULL};
  char *child_cmd = "complain";
  printf("PARENT: Junior is about to '%s', I'll keep an eye on him\n",
         child_cmd);
  pid_t child_pid = fork();

  // CHILD CODE
  if(child_pid == 0){
    printf("CHILD: I'm %d and I'm about to '%s'\n",
           getpid(), child_cmd);
    execvp(child_cmd, child_argv);
  }

  // PARENT CODE
  int status;
  int retcode = waitpid(child_pid, &status, WNOHANG);   // non-blocking wait 
  if(retcode == 0){                                     // 0 means child has not exited/changed status
    printf("PARENT: 0? The kid's not done yet. I'm bored\n");
  }
  else{                                                 // child has changed status / exited
    printf("PARENT: Something happend to junior!\n");
    if(WIFEXITED(status)){
      printf("Ah, he Exited with code %d", WEXITSTATUS(status));
    }
    else{ 
      printf("Junior didn't exit, what happened to him?\n");
    }
  }
  return 0;
}
  
