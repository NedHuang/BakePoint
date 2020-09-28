// Program demonstrating buffering effects of C stdio: printing
// functions may buffer output for later writing. If files aren't
// closed and proper exit doesn't occur, may truncate output.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  FILE *out = fopen("out.txt","w");
  for(int i=1; i<=1000; i++){   // Expect 1000 lines of output
    fprintf(out,"%04d\n",i);
  }

  // Any of the following will ensure full output makes it to the file
  // fflush(out);                  // flush file, don't close
  // fclose(out);                  // flush buffers and close file
  // exit(0);                      // exit normally

  // This however may cause data loss: less than 1000 lines in file
  _exit(0);                     // exit without any cleanup

}
