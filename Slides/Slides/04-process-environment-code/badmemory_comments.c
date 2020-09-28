// Includes some bad memory problems which are identified by valgrind
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv){
  int i,j;
  int k = i + j;
  printf("Uninitialized memory\n");
  if(k <200){
    printf("%d\n",k);		// Caught 
  }
  
  printf("Out of bounds on heap\n");
  int *ptr = malloc(sizeof(int)*4);
  for(i=0; i<= 4; i++){
    ptr[i] = i*i;		// Caught 
    printf("%d\n",ptr[i]);	
  }

  printf("Out of bounds on stack\n");
  int a[4];
  for(i=0; i<=(1<<3); i++){     // 1<<3 == 8, avoids compiler catch
    a[i] = i;
    printf("%d\n",a[i]);	// Not caught 
  }

  int *ip = NULL;
  printf("Last Num: %d\n",*ip); // Cause a crash 

  return 0;
}
