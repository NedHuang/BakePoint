// environment_vars.c: solution to in-class exercise showing how to
// check and set environment variables via the standard getenv() and
// setenv() functions.
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){

  char *rock = getenv("ROCK");
  if(rock == NULL){
    printf("ROCK not set\n");
  }
  else{
    printf("ROCK is %s\n",rock);
    printf("Turning VOLUME to 11\n");
    int fail = setenv("VOLUME","11",1);
    if(fail){
      printf("Couldn't change VOLUME\n");
    }
  }
  char *volume = getenv("VOLUME");
  if(volume == NULL){
    volume = "not set";
  }
  printf("VOLUME is %s\n",volume);
  return 0;
}
  

  
