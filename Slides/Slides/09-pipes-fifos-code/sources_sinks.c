// sources_sinks.c: Demonstrate how a FIFO coordinates reads/writes
// between multiple processes
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <assert.h>

#define BUFSIZE 1024

typedef struct {
  int from;
  int index;
} msg_t;

// Write from to source fd
void write_source(int fd, int source_num, int delay){
  int count = 0;
  while(1){
    msg_t msg = {.from=source_num, .index=count};
    int nbytes = write(fd, &msg, sizeof(msg_t));
    assert(nbytes == sizeof(msg_t));
    printf("SEND %d: Proc %d sent %d\n",
           source_num, getpid(), count);
    count++;
    sleep(delay);
  }
}

// Write to the source fd
void read_source(int fd, int sink_num, int delay){
  while(1){
    msg_t msg;
    int nbytes = read(fd, &msg, sizeof(msg_t));
    assert(nbytes == sizeof(msg_t));
    printf("RECV %d: Proc %d got {from=%3d, index=%3d}\n",
           sink_num, getpid(), msg.from, msg.index);
    sleep(delay);
  }
}


int main(int argc, char *argv[]){
  setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering

  if(argc < 3){
    printf("usage: %s <source_count> <sink_count>\n", argv[0]);
    exit(1);
  }

  int n_sources = atoi(argv[1]);
  int n_sinks   = atoi(argv[2]);

  // Create FIFO to coordinate
  remove("queue.fifo");         // remove if already present
  mkfifo("queue.fifo", S_IRUSR | S_IWUSR);
  int fifo_fd = open("queue.fifo", O_RDWR);
  printf("queue.fifo fd: %d\n",fifo_fd);

  // Create writing processes
  for(int i=1; i<=n_sources; i++){
    int pid=fork();
    if(pid==0){
      write_source(fifo_fd, i, 1);
    }
  }

  // Create reading processes
  for(int i=1; i<=n_sinks; i++){
    int pid=fork();
    if(pid==0){
      read_source(fifo_fd, i, 1);
    }
  }

  // Not likely to ever get beyond first wait
  for(int i=0; i<n_sources+n_sinks; i++){
    wait(NULL);
  }
  return 0;
}
