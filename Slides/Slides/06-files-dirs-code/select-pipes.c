// Use the select() syscall to do blocking waits for input from
// several sources. Several children are launched which sleep for
// different intervals then write to a pipe in an infinite loop. The
// parent uses select() to sleep until at least one 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>

#define COUNT  4
#define PREAD  0
#define PWRITE 1

// Run by child processes
void childish_activity(int childi, int write_fd);

int main(int argc, char *argv[]) {
  int pipes[COUNT][2];                                               // multiple pipes for children
  for(int i=0; i<COUNT; i++){
    pipe(pipes[i]);
  }

  int pids[COUNT];                                                   // pids of child processes
  for(int i=0; i<COUNT; i++){
    pids[i] = fork();
    if(pids[i] == 0){                                                // CHILD
      childish_activity(i, pipes[i][PWRITE]);                        // never returns from this function
    }
  }

  printf("Parent listening for children\n");                         // PARENT tracks children
  int maxfd = 0;                                                     // need maximum fd for select()
  while(1){                                                          // enter a loop to listen forever
    fd_set read_set;                                                 // set of file descriptors for select()
    FD_ZERO(&read_set);                                              // init the set
    for(int i=0; i<COUNT; i++){                                      // each iteration, reset to contain fds of interest
      FD_SET(pipes[i][PREAD], &read_set);                            // set fds for read ends of pipe
      maxfd = (maxfd < pipes[i][PREAD]) ? pipes[i][PREAD] : maxfd;   // must note the maximum fd
    }
    int nchild = select(maxfd+1, &read_set, NULL, NULL, NULL);       // sleep, wake up if any pipe ready for reading
    printf("%d children have something to say\n",nchild);            // report number of pipes with ready input

    for(int i=0; i<COUNT; i++){                                      // check each pipe
      if(FD_ISSET(pipes[i][PREAD], &read_set)){                      // checks if specific fd is set; if so input available
        char buf[1024];
        int n = read(pipes[i][PREAD], buf, 1024);                    // read from pipe
        buf[n] = '\0';                                               // null terminate
        printf("%d has %d bytes of data: |%s|\n",                    // put output on screen
               pids[i], n, buf);
      }
    }
  }

  exit(0);
}

// Enter a loop that repeatedly sleeps then writes to the given file
// descriptor infinitely.
void childish_activity(int childi, int write_fd){
  while(1){
    char buf[1024];
    sleep( childi%2+1 );                                             // sleep dependent on childi
    snprintf(buf,1024,"%d: I'm Mr. %d-seeks, look at me!",           // create messge
             getpid(),childi);
    write(write_fd, buf, strlen(buf));                               // write message to pipe

    sleep( 1 );                                                      // sleep always 1 sec
    snprintf(buf,1024,"%d: I'm Mr. %d-seeks, look at me!",           // create message
             getpid(),childi);
    write(write_fd, buf, strlen(buf));                               // write to pipe
  }
}

