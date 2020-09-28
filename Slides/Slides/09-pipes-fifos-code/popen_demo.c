// Uses convenience functions popen() and pclose() to send contents of
// a file to a child process via standard in.
// 
// Adapted from Stevens/Rago Figure 15.11 "Copy file to pager program
// using popen"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
  if (argc < 3){
    printf("usage: %s <file> <program>\n",argv[0]);
    exit(1);
  }

  char line[MAXLINE];
  char *child_program = argv[2];

  FILE *fpin = fopen(argv[1], "r");             // open a file to read its contents
  if (fpin == NULL){                            // check for errors
    perror("can′t open input file");
    exit(1);   
  }
  FILE *fpout = popen(child_program, "w");      // open a start child program with pipe connected to its stdin
  if (fpout == NULL){                           // check for errors
    perror("popen error");
    exit(1);
  }

  while (fgets(line, MAXLINE, fpin) != NULL){   // copy contents of file to child program pipe 
    if (fputs(line, fpout) == EOF){
      perror("fputs error to pipe");
      exit(1);
    }
  }
  // if (ferror(fpin)){                            // check 
  //   perror("fgets error");
  //   exit(1);
  // }
  if (pclose(fpout) == -1){                     // close pipe and wait() on child program
    perror("pclose error");
    exit(1);
  }

  return 0;
}
