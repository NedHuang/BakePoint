// Outputs contents of file and connects a pipe to a child process
// which runs the "less" pager program.
// 
// Adapted from Stevens/Rago Figure 15.11 "Copy file to pager program
// using popen"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define PAGER   "less" // environment variable, or default 
// #define PAGER   "${PAGER:-more}" // environment variable, or default 

int main(int argc, char *argv[]) {
  char    line[MAXLINE];
  FILE    *fpin, *fpout;

  if (argc != 2){
    printf("usage: a.out <file>\n");
    exit(1);
  }

  if ((fpin = fopen(argv[1], "r")) == NULL){
    perror("can′t open input file");
    exit(1);
  }

  if ((fpout = popen(PAGER, "w")) == NULL){
    perror("popen error");
    exit(1);
  }

  // copy argv[1] to pager
  while (fgets(line, MAXLINE, fpin) != NULL) {
    if (fputs(line, fpout) == EOF){
      perror("fputs error to pipe");
      exit(1);
    }
  }
  if (ferror(fpin)){
    perror("fgets error");
    exit(1);
  }
  if (pclose(fpout) == -1){
    perror("pclose error");
    exit(1);
  }

  return 0;
}
