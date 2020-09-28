#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv){
  int i,j;
  int k = i + j;
  if(k <200){
    printf("%d\n",k);
  }
  int *ptr = malloc(sizeof(int)*4);
  for(i=0; i<= 4; i++){
    ptr[i] = i*i;
    printf("%d\n",ptr[i]);	
  }
  int a[4];
  for(i=0; i<=(1<<3); i++){
    a[i] = i;
    printf("%d\n",a[i]);
  }
  int *ip = NULL;
  printf("Last Num: %d\n",*ip);
  return 0;
}
