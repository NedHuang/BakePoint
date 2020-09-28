// shmdemo_posix.c: demonstrate posix shared memory segments, very
// similar to memory mapped files. Program creates a segement of
// shared memory called '/something_shared' and either reports its
// current contents or writes new contents to it.  On Linux, this
// shared segment looks like a regular file in
// 
// /dev/shm/something_shared
// 
// > gcc -o shmdemo shmdemo_posix.c -lrt

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <string.h>

#define SHM_SIZE 1024                                     // size of shared memory, 1K

int main(int argc, char *argv[]) {
  if (argc > 2) {
    printf("usage: %s\n",argv[0]);
    printf("  print contents of shared memory segment\n");
    printf("usage: %s [data_to_write]\n",argv[0]);
    printf("  write new contents of to shared memory segment\n");
    printf("usage: %s -watch\n",argv[0]);
    printf("  report shared memory contents every second\n");
    printf("usage: %s -delete\n",argv[0]);
    printf("  delete shared memory segment\n");
    exit(1);
  }

  char *shared_name = "/something_shared";
  if (argc == 2 && strcmp(argv[1],"-delete")==0 ) {
    printf("removing shared memory\n");
    shm_unlink(shared_name);                              // unlink and remove the segment
    return 0;
  }

  int shared_fd =                                         // retrieve a file descriptor for shared memory
    shm_open(shared_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  ftruncate(shared_fd, SHM_SIZE);                         // set the size of the shared memory area

  char *shared_bytes =                                    // map into process address space
    mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
  close(shared_fd);

  if (argc == 2 && strcmp(argv[1],"-watch")==0 ) {        // watch memory, report every second
    while(1){
      printf("segment contains: '%s'\n", shared_bytes);   
      sleep(1);
    }
    return 0;
  }

  else if (argc == 2 ) {
    printf("writing to segment: '%s'\n", argv[1]);        // modify the segment based on the command line
    strncpy(shared_bytes, argv[1], SHM_SIZE);             // copy characters into the shared memeory
  }
  else{
    printf("segment contains: '%s'\n", shared_bytes);     // retrieve current contents once
  }
  munmap(shared_bytes, SHM_SIZE);                         // unmap the shared meory

  return 0;
}
