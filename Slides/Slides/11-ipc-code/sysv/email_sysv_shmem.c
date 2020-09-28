// email_sysv_shmem.c: demonstration of establishing shared memory
// array using System V shmem. Stores the data for name/email records
// in the a System V shared memory segment that remains on the system
// after the program exits. Subsequent calls to the program can lookup
// or alter this block of shared memory. The ipcs command is useful to
// list and show the IPC resources and ipcrm can remove those
// resources.
// 
// SAMPLE USAGE:
// 
// > gcc -o email email_sysv_shmem.c
// > ./email restore
// Restoring shared memory 129957905
// > ipcs
// 
// ------ Message Queues --------
// key        msqid      owner      perms      used-bytes   messages    
// ...
// 
// ------ Shared Memory Segments --------
// key        shmid      owner      perms      bytes      nattch     status      
// 0x52040681 129957905  kauffman   644        5376       0                       
// 
// ------ Semaphore Arrays --------
// key        semid      owner      perms      nsems     
// ...
// 
// > email lookup 'Chris Kauffman'
// Looking up Chris Kauffman
// Found: kauffman@umn.edu
// 
// > email lookup 'Michael Steinbach'
// Looking up Michael Steinbach
// Found: steinbac@cs.umn.edu
// 
// > email change 'Michael Steinbach' 'mikes@umn.edu'
// Changing 'Michael Steinbach' to 'mikes@umn.edu'
// Alteration complete
// 
// > email lookup 'Michael Steinbach'
// Looking up Michael Steinbach
// Found: mikes@umn.edu
// 
// > ./email restore
// Restoring
// 
// > email lookup 'Michael Steinbach'
// Looking up Michael Steinbach
// Found: steinbac@cs.umn.edu
//
// > ipcrm -m 129957905
// 
// > email lookup 'Chris Kauffman'
// Looking up Chris Kauffman
// Not found


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NRECS 21
#define STRSIZE 128
#define SHM_SIZE (NRECS * sizeof(lookup_t))

// structure to store a lookup_t of name-to-email association
typedef struct {
  char name [STRSIZE];
  char email[STRSIZE];
} lookup_t;

lookup_t original_data[NRECS] = {
  {"Chris Kauffman"       ,"kauffman@umn.edu"},
  {"Christopher Jonathan" ,"jonat003@umn.edu"},
  {"Amy Larson"           ,"larson@cs.umn.edu"},
  {"Chris Dovolis"        ,"dovolis@cs.umn.edu"},
  {"Dan Knights"          ,"knights@cs.umn.edu"},
  {"George Karypis"       ,"karypis@cs.umn.edu"},
  {"Steven Jensen"        ,"sjensen@cs.umn.edu"},
  {"Daniel Keefe"         ,"dfk@umn.edu"},
  {"Michael W. Whalen"    ,"whalen@cs.umn.edu"},
  {"Catherine Qi Zhao"    ,"qzhao@umn.edu"},
  {"Dan Challou"          ,"challou@cs.umn.edu"},
  {"Steven Wu"            ,"zsw@umn.edu"},
  {"Michael Steinbach"    ,"steinbac@cs.umn.edu"},
  {"Jon Weissman"         ,"jon@cs.umn.edu"},
  {"Victoria Interrante"  ,"interran@cs.umn.edu"},
  {"Shana Watters"        ,"watt0087@umn.edu"},
  {"James Parker"         ,"jparker@cs.umn.edu"},
  {"James Moen"           ,"moen0017@cs.umn.edu"},
  {"Daniel Giesel"        ,"giese138@umn.edu"},
  {"Jon Read"             ,"readx028@umn.edu"},
  {"Sara Stokowski"       ,"stoko004@umn.edu"},
};


int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("usage: %s restore\n",argv[0]);
    printf("       %s lookup <name>\n",argv[0]);
    printf("       %s change <name> <email>\n",argv[0]);
    exit(1);
  }

  key_t key  = ftok("shmdemo.c", 'R');                      // make the SysV IPC key
  int shmid  = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);     // connect to (and possibly create) the segment:
  lookup_t *data = (lookup_t *) shmat(shmid, (void *)0, 0); // attach to the segment to get a pointer to it:

  if( strcmp("restore",argv[1])==0 ){
    printf("Restoring shared memory %d\n",shmid);
    for(int i=0; i<NRECS; i++){
      data[i] = original_data[i];
    }
  }
  else if( strcmp("lookup",argv[1])==0 ){
    char *name = argv[2];
    printf("Looking up %s\n",name);
    int found = 0;
    for(int i=0; i<NRECS; i++){
      if(strcmp(name, data[i].name) == 0){
        printf("Found: %s\n",data[i].email);
        found = 1;
      }
    }
    if(!found){
      printf("Not found\n");
    }
  }
  else if( strcmp("change",argv[1])==0 ){
    char *name = argv[2];
    char *email = argv[3];
    printf("Changing '%s' to '%s'\n",name,email);
    int found = 0;
    for(int i=0; i<NRECS; i++){
      if(strcmp(name, data[i].name) == 0){
        strcpy(data[i].email, email);
        printf("Alteration complete\n");
        found = 1;
      }
    }
    if(!found){
      printf("Not found\n");
    }
  }
  else{
    printf("Unknown command '%s'\n",argv[1]);
    shmdt(data);
    exit(1);
  }

  shmdt(data);                                            // detach from the shared meory
  exit(0);
}
