// mixed_std_low.c: Program demonstrating strange effects of mixing C
// standard I/O like fscanf() and Unix system I/O like read(). Typical
// output is
//
// FIRST: 1
// NEXT: 41 1042 1043 1044 1045 1046 1047 1048 1049 1050 1051 1052 1053 
//
// which is explained by the internal buffering of C standard I/O.
#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[]){
  FILE *input = fopen("3K.txt","r");
  // // Uncomment to turn off buffering of stdin
  // setvbuf(input, NULL, _IONBF, 0);

  int first;
  fscanf(input, "%d", &first);
  printf("FIRST: %d\n",first);

  int fd = fileno(input);
  char *buf[64];
  read(fd, buf, 63);
  buf[63] = '\0';
  printf("NEXT: %s\n",buf);

  return 0;
}
