// pause_server.c: get 4 client connections then inform them of
// shutdown. Solution to in-class exercise.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>

void get_address_string(struct addrinfo *addr, char buffer[]);
void get_sock_address_string(struct sockaddr *addr, char buffer[]);
void get_sock_fd_address_string(int sockfd, char buffer[]);

int main(void) {
  int ret;
  char address_str[INET6_ADDRSTRLEN+16];                // fill in a string version of the addresses for printing

  struct addrinfo hints = {                             // hints allow a specific kind of socket
    .ai_family =   AF_UNSPEC,                           // to be set up for the server
    .ai_socktype = SOCK_STREAM,
    .ai_flags =    AI_PASSIVE,
  };

  #define PORT "12344"                                  // port number clients will connect to
  struct addrinfo *serv_addr;                           // address of the server
  ret = getaddrinfo(NULL, PORT, &hints, &serv_addr);    // find server (my) address
  assert(ret == 0);

  int listen_fd = socket(serv_addr->ai_family,          // fd of socket on which the server will listen
                         serv_addr->ai_socktype,
                         serv_addr->ai_protocol);
  assert(listen_fd != -1);

  int yes = 1;
  ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, // set socket options to allow re-use of address
                   &yes, sizeof(int));                  // avoids "Address already in use" errors on bind
  assert(ret != -1);

  ret = bind(listen_fd,                                 // bind the socket to the server address given
             serv_addr->ai_addr,                        // allows listening for connections later on
             serv_addr->ai_addrlen);
  if(ret == -1){ perror("bind failed"); exit(1); }


  get_address_string(serv_addr, address_str);
  printf("SERVER: address is %s\n", address_str);

  freeaddrinfo(serv_addr);                              // no longer need the server address once bind completes

  printf("SERVER: waiting for connections on port %s\n", PORT);


  #define MAX_CLIENTS 4
  int client_fds[MAX_CLIENTS];

  #define BACKLOG 10                                  // number of pending connections to queue before refusing
  ret = listen(listen_fd, BACKLOG);                   // block until a client tries to connect

  struct sockaddr_storage client_addr_storage;        // info on clients that may connect and
  struct sockaddr *client_addr =                      // structs to store the connection data
    (struct sockaddr *) &client_addr_storage;  
  socklen_t client_addr_size = sizeof(client_addr);

  for(int i=0; i<MAX_CLIENTS; i++){
    int client_fd = accept(listen_fd,                   // accept a connection from the open port from a client
                           client_addr,                 // produces a new file descriptor for socket created to communicate
                           &client_addr_size);          // with the client and fills in client addrss info
    assert(client_fd != -1);

    get_sock_address_string(client_addr, address_str);
    printf("SERVER: connection %d from %s\n", i, address_str);

    client_fds[i] = client_fd;
  }


  for(int i=0; i<MAX_CLIENTS; i++){                     // send each client shutdown messages
    int client_fd = client_fds[i];
    get_sock_fd_address_string(client_fd, address_str);

    printf("SERVER: sending shutdown to %s\n",address_str);

    char *msg = "Server shutting down.";
    ret = write(client_fd, msg, strlen(msg));
    assert(ret != -1);
    close(client_fd);
  }

  close(listen_fd);                                     // should close socket on shutdown though this is unreachable
  return 0;
}

// Fill in the given buffer with a string version of the address from
// the given addrinfo.  This involves some nasty casting.  addrinfo
// structs have the member.
// 
//  struct sockaddr *ai_addr;	/* Socket address for socket.  */
// 
// which may be either a sockaddr_in (IPv4) or sockaddr_in6 (IPv6).
// The exact type is defined in the sin_family which is supposed be
// identical to the ai_family field of the addrinfo.
//
// The main workhorse is inet_ntop() which does the actual fill-in of
// the buffer with the translated address.
void get_address_string(struct addrinfo *addr, char buffer[]){
  void *ip_address = NULL;
  if(addr->ai_family == AF_INET){               // ipv4 address
    ip_address = (void *) &((struct sockaddr_in*)addr->ai_addr)->sin_addr;
  }
  else if(addr->ai_family == AF_INET6){         // ipv6 address
    ip_address = (void *) &((struct sockaddr_in6*)addr->ai_addr)->sin6_addr;
  }
  inet_ntop(addr->ai_family, ip_address, buffer, INET6_ADDRSTRLEN);
}

// Same as above but takes a socket address as an argument rather than
// a full addrinfo. Also adds the port number to the string.
// Called like the below:
// 
// get_sock_address_string(serv_addr->ai_addr, address_str);   
void get_sock_address_string(struct sockaddr *addr, char buffer[]){
  void *ip_address = NULL;
  if(addr->sa_family == AF_INET){               // ipv4 address
    ip_address = (void *) &(((struct sockaddr_in *) addr)->sin_addr);
  }
  else if(addr->sa_family == AF_INET6){         // ipv6 address
    ip_address = (void *) &(((struct sockaddr_in6 *) addr)->sin6_addr);
  }
  inet_ntop(addr->sa_family, ip_address, buffer, INET6_ADDRSTRLEN);
  int len = strlen(buffer);
  struct sockaddr_in *addr_in = (struct sockaddr_in *) addr;
  sprintf(buffer+len,":%d",addr_in->sin_port);          // add on the port for printing
}

void get_sock_fd_address_string(int sockfd, char buffer[]){
  struct sockaddr_storage client_addr_storage;        // info on clients that may connect and
  struct sockaddr *client_addr =                      // structs to store the connection data
    (struct sockaddr *) &client_addr_storage;  
  socklen_t client_addr_size = sizeof(client_addr);
  int ret = getpeername(sockfd,                         // recover the address of the the peer side of the socket
                        client_addr,
                        &client_addr_size);  
  assert(ret != -1);
  get_sock_address_string(client_addr, buffer);
}
