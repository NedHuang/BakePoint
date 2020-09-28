// baudelair_orphans.c: demonstrate creation of orphan processes: the
// parent process forks() 3 child processes but exits without waiting
// for them.  Several or all of these child processes will become
// orphans reporting their current parent as '1', the root process
// that adopts all orphans.
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv){
  int i;
  pid_t child_pid;
  for(i=0; i<3; i++){
    child_pid = fork();
    if(child_pid == 0){
      break;                    // why break here?
    }
  }
  pid_t pid = getpid();
  pid_t orig_parent = getppid();

  if(child_pid != 0){           // PARENT CODE
    sleep(1);                   // sleep a moment
    return 0;                   // exit, likely orphaning children
  }

  if(i==0){                     // CHILD CODE
    printf("%d: I'm Klaus and my parent is %d\n",
           pid, orig_parent);
  }
  else if(i==1){                // CHILD CODE
    printf("%d: I'm Violet and my parent is %d\n",
           pid, orig_parent);
  }
  else if(i==2){                // CHILD CODE
    printf("%d: (Sunny blows raspberry) %d\n",
           pid, orig_parent);
  }
  sleep(i+1);                   // CHILD CODE continues

  pid_t cur_parent=getppid();
  printf("%d: My original parent was %d, my current parent is %d\n",
         pid, orig_parent, cur_parent);

  if(cur_parent != orig_parent){
    printf("%d: I've been orphaned. How Unforunate.\n", pid);
  }

  return 0;
}
