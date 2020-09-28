// msg_spock_posix.c: demonstration of using POSIX message queues to
// allow two unrelated programs to communicate, kirk/spock in this
// case. Each has an incoming message queue to which it responds.  Run
//
// # COMPILE
// > gcc -o kirk msg_kirk_posix.c -lrt
// > gcc -o spock msg_spock_posix.c -lrt
//
// Run each of kirk/spock in their own terminal.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAX_LEN 128

int main(void){
  struct mq_attr attr = {
    .mq_maxmsg = 10,
    .mq_msgsize = MAX_LEN,
  };
  mqd_t enterprise_queue_descriptor =
    mq_open("/enterprise", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &attr);
  mqd_t kirk_queue_descriptor =
    mq_open("/kirk", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &attr);

  // if(queue_descriptor == -1){
  //   perror("couldn't open message queue");
  //   exit(1);
  // }

  printf("spock: ready to receive messages, captain.\n");

  // Spock never quits: forever wait for messages in the queue
  char msgbuf[MAX_LEN];
  while(1){
    int received = mq_receive(enterprise_queue_descriptor, msgbuf, MAX_LEN, NULL);
    if (received == -1) {
      perror("mq_receive failed");
      exit(1);
    }
    printf("spock received: '%s'\n", msgbuf);

    char msgbuf2[MAX_LEN];
    snprintf(msgbuf2,MAX_LEN,"Captain: '%s' is highly illogical\n",msgbuf);

    int result = mq_send(kirk_queue_descriptor, msgbuf2, MAX_LEN, 0);
    if (result == -1){
      perror("mq_send failed");
      exit(1);
    }
  }

  mq_close(enterprise_queue_descriptor);
  mq_close(kirk_queue_descriptor);
  return 0;
}

