// Demonstrate basic C functions for directory manipulation. ALL of
// the calls below should do error checking but doing so makes the
// code harder to read.
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#define SIZE 2048
#define TMPDIR "tmpdir"

int main(int argc, char *argv[]){
  int ret;
  char buf[SIZE];
  mode_t perms = S_IRUSR | S_IWUSR | S_IXUSR;

  getcwd(buf,SIZE);
  printf("WORKING DIRECTORY: %s\n\n",buf);

  mkdir(TMPDIR,perms);
  printf("MADE %s\n\n",TMPDIR);

  printf("CHANGING TO %s\n\n",TMPDIR);
  chdir(TMPDIR);

  getcwd(buf,SIZE);
  printf("WORKING DIRECTORY: %s\n\n",buf);

  printf("CHANGING TO ..\n\n",TMPDIR);
  chdir("..");

  getcwd(buf,SIZE);
  printf("WORKING DIRECTORY: %s\n\n",buf);

  printf("DIRECTORY LISTING\n\n");
  DIR *dir = opendir(".");
  struct dirent *file = NULL;
  while( (file = readdir(dir)) != NULL){
    printf("%s\n", file->d_name);
  }
  closedir(dir);

  printf("\nREMOVING %s\n\n",TMPDIR);
  rmdir(TMPDIR);

  printf("DIRECTORY LISTING\n\n");
  dir = opendir(".");
  while( (file = readdir(dir)) != NULL){
    printf("%s\n", file->d_name);
  }
  closedir(dir);

  return 0;
}
