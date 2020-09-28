// Demonstrate that full buffering can make output/input trickier: the
// prompt will not display under full buffering
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  char outbuf[1024] = {};
  setvbuf(stdout, outbuf, _IOFBF, 1024); // set full buffering for stdout
  // setvbuf(stdout, outbuf, _IOLBF, 1024); // set line buffering for stdout

  fprintf(stdout,"Enter number: ");
  int num = -1;
  fscanf(stdin,"%d",&num);
  fprintf(stdout,"Number is %d\n",num);


  char outbuf2[1024] = {};
  setvbuf(stdout, outbuf2, _IOFBF, 1024); // set full buffering for stdout
  fprintf(stdout,"BUFFER IS:\n");
  for(int i=0; i<1024; i++){
    fprintf(stdout,"%c",outbuf[i]);
  }
  fprintf(stdout,"\n");
  return 0;
}

// char *msg = "Enter number: ";
// int len = strlen(msg);
// for(int i=0; i<len; i++){
//   fputc(msg[i],stdout);
// }
