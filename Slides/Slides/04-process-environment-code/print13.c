// print13.c: Prints 1st and 3rd command line arguments without
// checking they are there. May kersplode if insufficient args are
// provided.
#include <stdio.h>
int main(int argc, char *argv[]){
  printf("%s\n",argv[1]);
  printf("%s\n",argv[3]);
}
