// spawn_undead.c: demonstrate creation of zombie processes: children
// complete but are stay around until a wait() call from their parent
// process before then can terminate completely. These will have staus
// 'Z' in process listings such as with top.
// 
// In one terminal compile to executable 'spawn_undead' and run. In
// another terminal, run the command
// 
// $> watch -n 0.1 'ps u -C spawn_undead'
//
// which will monitor for processes with that name showing
// information. On most linux systems, the child process will
// initially have status 'S' for suspended due to the sleep() call but
// will then switch to 'Z' as it completes but has not been reaped by
// a parent wait() call.
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv){

  pid_t child_pid = fork();

  // CHILD CODE
  if(child_pid == 0){
    sleep(1);
    printf("ZOMBIE %d: Brains...\n", getpid());
    exit(0);
  }

  // PARENT CODE
  printf("NECROMANCER: Go forth my servant #%d...\n",child_pid);
  printf("NECROMANCER: Now for a nap...\n");
  sleep(10);
  
  printf("NECROMANCER: I awaken and reap thee, my loyal cadaver.\n");
  wait(NULL);
  return 0;
}
  
