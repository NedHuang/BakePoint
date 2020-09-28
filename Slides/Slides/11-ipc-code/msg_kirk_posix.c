// msg_kirk_posix.c: demonstration of using POSIX message queues to
// allow two unrelated programs to communicate, kirk/spock in this
// case. Each has an incoming message queue to which it responds.  Run
//
// # COMPILE
// > gcc -o kirk msg_kirk_posix.c -lrt
// > gcc -o spock msg_spock_posix.c -lrt
//
// Run each of kirk/spock in their own terminal.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAX_LEN 128

int main(){
  struct mq_attr attr = {
    .mq_maxmsg = 10,
    .mq_msgsize = MAX_LEN,
  };
  mqd_t enterprise_queue_descriptor =
    mq_open("/enterprise", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &attr);
  mqd_t kirk_queue_descriptor =
    mq_open("/kirk", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &attr);

  printf("Enter lines of text, ^D to quit:\n");

  char msgbuf[MAX_LEN];

  // Repeatedly read a line of text into the message buffer and put it into the queue
  while( 1 ){
    char *input = fgets(msgbuf, MAX_LEN, stdin);
    if(input == NULL){
      break;
    }
    int len = strlen(msgbuf);
    msgbuf[len-1] = '\0';     // ditch newline at end, if it exists 
    printf("sending '%s'\n",msgbuf);
    int result = mq_send(enterprise_queue_descriptor, msgbuf, MAX_LEN, 0);
    if (result == -1){
      perror("mq_send failed");
      exit(1);
    }

    int received = mq_receive(kirk_queue_descriptor, msgbuf, MAX_LEN, NULL);
    if (received == -1) {
      perror("mq_receive failed");
      exit(1);
    }
    printf("enterprise responded: %s\n",msgbuf);
  }

  mq_close(enterprise_queue_descriptor);
  mq_close(kirk_queue_descriptor);
  return 0;
}

