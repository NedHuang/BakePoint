#include <stdlib.h>
int glob1 = 2;                  // global var
int func(int *a){               // param stack var
  int b = 2 * (*a);             // local stack var
  return b;                     // de-allocate locals in func()
}
int main(){                     // main entry point
  int x = 5;                    // local stack var
  int c = func(&x);             // local stack var
  int *p = malloc(sizeof(int)); // local stack var that points into heap 
  *p = 10;                      // modify heap memory
  glob1 = func(p);              // allocate func() locals and run code
  free(p);                      // deallocate heap mem pointed to p
  return 0;                     // deallocate locals in main()
}
// all executable code is in the .text memory area as assmebly instructions
