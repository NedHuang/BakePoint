// System V message queues: writes to a a message queue, sends
// receives a response.
// 
// Based on: http://beej.us/guide/bgipc/output/html/multipage/mq.html

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Custom struct to hold messages in the message queue
struct my_msgbuf {
  long mtype;                   // tag allowing filtering of messages
  char mtext[200];              // Contents of message
};

int main(){
  // Get a unique IPC key based on a file name
  key_t key = ftok("kirk.c", 'B');
  if(key == -1){
    perror("ftok");
    exit(1);
  }

  // Get a message queue based on the key provided
  int msqid = msgget(key, 0644 | IPC_CREAT);
  if(msqid == -1) {
    perror("msgget");
    exit(1);
  }
	
  struct my_msgbuf buf;
  buf.mtype = 1;                // all messages have the same tag, no filtering done in this program

  printf("Enter lines of text, ^D to quit:\n");

  // Repeatedly read a line of text into the message buffer and put it into the queue
  while(fgets(buf.mtext, sizeof(buf.mtext), stdin) != NULL) {
    int len = strlen(buf.mtext);

    // ditch newline at end, if it exists 
    if (buf.mtext[len-1] == '\n'){
      buf.mtext[len-1] = '\0';
    }
    int sent = msgsnd(msqid, &buf, len+1, 0);
    if (sent == -1){
      perror("msgsnd");
    }

    // Expect an acknowledgement
    int received = msgrcv(msqid, &buf, sizeof buf.mtext, 0, 0);
    if (received == -1) {
      perror("msgrcv");
      exit(1);
    }
    printf("Got back: %s\n",buf.mtext);
  }

  // Eliminate the message queue
  int removal = msgctl(msqid, IPC_RMID, NULL);
  if (removal == -1) {
    perror("msgctl");
    exit(1);
  }

  return 0;
}

