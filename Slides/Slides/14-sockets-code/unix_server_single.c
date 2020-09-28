// unix_server_single.c: demonstration of Unix domain sockets, local
// sockets which are based on a file on the file system as the
// 'address' of the socket. The server listens for a SINGLE
// connections from clients and echos back the text that they
// send. Only a single client at a time is serviced.
// 
// Adapted from: https://github.com/troydhanson/network/blob/master/unixdomain/01.basic/srv.c

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("usage: %s <sockfile>\n",argv[0]);
    printf("  sockfile is the name of a file which will be used for the socket\n");
    return 1;
  }

  char *sockfile = argv[1];
  remove(sockfile);
  int connect_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  struct sockaddr_un addr = {
    .sun_family = AF_UNIX,      // UNIX domain socket tied to a file
    .sun_path   = "",           // need to strcpy this
  };
  strcpy(addr.sun_path, sockfile);

  bind(connect_sockfd, (struct sockaddr*)&addr, sizeof(addr));

  #define BACKLOG 10            // number of pending connections to queue before refusing
  listen(connect_sockfd, BACKLOG);     // set the socket to liste for clients


  printf("waiting for a client\n");
  int client_fd = accept(connect_sockfd, NULL, NULL);
  printf("client connection established\n");

  #define BUFSIZE 128
  char buf[BUFSIZE];
  int nchars = snprintf(buf, BUFSIZE, "server %d listening for input", getpid());
  write(client_fd, buf, nchars);

  while(1){
    nchars = read(client_fd, buf, BUFSIZE);
    if(nchars == 0){
      break;
    }
    printf("read %u bytes: %.*s\n", nchars, nchars, buf);
  }
  close(client_fd);
  printf("client connection closed\n");

  close(connect_sockfd);
  return 0;
}
