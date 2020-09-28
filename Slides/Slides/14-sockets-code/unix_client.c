// unix_client.c: demonstration of Unix domain sockets, local sockets
// which are based on a file on the file system as the 'address' of
// the socket. The client connects to the running unix_server and
// reads data from standard input passing it to the server.
// 
// Adapted from: https://github.com/troydhanson/network/blob/master/unixdomain/01.basic/cli.c

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("usage: %s <sockfile>\n",argv[0]);
    printf("  sockfile is the name of a file which will be used for the socket\n");
  }

  char *sockfile = argv[1];
  if( access(sockfile, F_OK) != 0 ){
    printf("Cant access socket file '%s' - is the server running?\n",
           sockfile);
    exit(1);
  }

  int client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un addr = {
    .sun_family = AF_UNIX,      // UNIX domain socket tied to a file
    .sun_path   = "",           // need to strcpy this
  };
  strcpy(addr.sun_path, sockfile);

  connect(client_sockfd, (struct sockaddr*)&addr, sizeof(addr));

  #define BUFSIZE 128
  char buf[BUFSIZE];

  int nchars = read(client_sockfd, buf, BUFSIZE);       // accept a server message
  printf("read %u bytes: %.*s\n", nchars, nchars, buf); // 'server XXXX listening for input'

  nchars = snprintf(buf, BUFSIZE, "client process %d connected", getpid());
  write(client_sockfd, buf, nchars);              // send a message to the server

  while(1){
    nchars = read(STDIN_FILENO, buf, BUFSIZE);
    if(nchars == 0){
      break;
    }
    write(client_sockfd, buf, nchars);
  }
  close(client_sockfd);

  return 0;
}
