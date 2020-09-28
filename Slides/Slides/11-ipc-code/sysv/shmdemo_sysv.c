// System V Shared Memory demo. Create a piece of shared memory, write
// to it or retrive from it.
//
// usage: shmdemo        - prints current contents
//        shmdemo <arg>  - changes contents to be the argument given
// 
// Based on 9.6 of http://beej.us/guide/bgipc/output/html/multipage/shm.html
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024                                     // size of shared memory, 1K

int main(int argc, char *argv[]) {
  if (argc > 2) {
    fprintf(stderr, "usage: shmdemo [data_to_write]\n");
    exit(1);
  }

  // All error handling in the below has been eliminated
  key_t key  = ftok("crap", 'R');                       // make the SysV IPC key
  int shmid  = shmget(key, SHM_SIZE, 0644 | IPC_CREAT); // connect to (and possibly create) the segment:
  char *data = shmat(shmid, (void *)0, 0);              // attach to the segment to get a pointer to it:

  if (argc == 2) {
    printf("writing to segment: \"%s\"\n", argv[1]);      // modify the segment based on the command line
    strncpy(data, argv[1], SHM_SIZE);                     // copy characters into the shared memeory
  }
  else{
    printf("segment contains: \"%s\"\n", data);           // retrieve current contents
  }
  shmdt(data);                                            // detach from the shared meory

  return 0;
}
