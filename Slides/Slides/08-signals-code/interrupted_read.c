// interrupted_read.c: In older unix systems, system calls like read()
// could be interrupted by a signal and would return an error
// indicating that the call did not complete due to the signal. This
// requried every system call to be followed by error checking and
// retrying to account for signals. In modern systems, system calls
// are usually restarted automatically so this program is for demo
// purposes only.
// 
// Easiest to run this program using a fifo as in
// > mkfifo not-there-yet.fifo
// > gcc interrupted_read.c
// > a.out not-there-yet.fifo
//
// In a separate terminal send signals to the process such as SIGUSR1
// then echo data into the fifo.
//
// > pkill -USR1 a.out
// > pkill -USR2 a.out
// > echo 'Here is some text' > not-there-yet.fifo

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void handle_usr_signals(int signo) {            // argument is signal number 
  if (signo == SIGUSR1){                        // check for signal type 1
    printf("received SIGUSR1\n");
  }
  else if (signo == SIGUSR2){                   // check for signal type 2
    printf("received SIGUSR2\n");
  }
  else{                                         // this should not be reachable
    printf("signo %d: Wait, how did I get here?\n",signo);
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  signal(SIGUSR1, handle_usr_signals);
  signal(SIGUSR2, handle_usr_signals);  

  char *fname = argv[1];

  int fd = open(fname,O_RDONLY);
  int nread;
  char buf[512];
  while( (nread = read(fd, buf, 512)) == -1){   // Keep reading
    if(errno == EINTR){                         // Check whether we were interrupted
      printf("Interrupted while reading.\n");
    }
    else{
      perror("reading failed due to error");
      exit(1);
    }
  }

  printf("Read the following: ");
  fflush(stdout);
  write(STDOUT_FILENO, buf, nread);
  printf("\n");
  exit(0);
}
