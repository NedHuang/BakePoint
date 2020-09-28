// overflow.c: program traverses memory that it really ought not to by
// walking off the end of an array into parts unknown.

#include <stdio.h>
int main(int argc, char *argv[]){
  char a[3] = {'A','B','C'};
  int i = 0;
  while(1){
    printf("%c",a[i]);
    i++;
    if(i%40 == 0){
      printf("\n");
    }
  }
  return 0;
}
