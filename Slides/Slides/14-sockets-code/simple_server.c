// simple_server.c: demonstration server

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


#define PORT "12344"                                    // port number clients will connect to


void get_address_string(struct addrinfo *addr, char buffer[]);
void get_sock_address_string(struct sockaddr *addr, char buffer[]);

int main(void) {
  int ret;

  struct addrinfo hints = {                             // hints allow a specific kind of socket
    .ai_family =   AF_UNSPEC,                           // to be set up for the server
    .ai_socktype = SOCK_STREAM,
    .ai_flags =    AI_PASSIVE,
  };

  struct addrinfo *serv_addr;                           // address of the server
  ret = getaddrinfo(NULL, PORT, &hints, &serv_addr);    // find server (my) address, not NULL for 1st arg
  assert(ret == 0);

  int listen_fd = socket(serv_addr->ai_family,          // fd of socket on which the server will listen
                         serv_addr->ai_socktype,
                         serv_addr->ai_protocol);
  assert(listen_fd != -1);

  int yes = 1;
  ret = setsockopt(listen_fd, SOL_SOCKET,               // set socket options to allow re-use of address
                   SO_REUSEADDR, &yes, sizeof(int));    // avoids "Address already in use" errors on bind
  assert(ret != -1);

  ret = bind(listen_fd,                                 // bind the socket to the server address given
             serv_addr->ai_addr,                        // allows listening for connections later on
             serv_addr->ai_addrlen);
  if(ret == -1){ perror("bind failed"); exit(1); }


  char address_str[INET6_ADDRSTRLEN];                   // fill in a string version of the server address for printing
  get_address_string(serv_addr, address_str);
  printf("SERVER: address is %s\n", address_str);

  freeaddrinfo(serv_addr);                              // no longer need the server address once bind completes

  printf("SERVER: waiting for connections on port %s\n", PORT);

  #define BACKLOG 10                                    // number of pending connections to queue before refusing
  ret = listen(listen_fd, BACKLOG);                     // block until a client tries to connect

  while(1){                                             // server listens forever
    struct sockaddr_storage client_addr_storage;        // info on clients that may connect and
    struct sockaddr *client_addr =                      // structs to store the connection data
      (struct sockaddr *) &client_addr_storage;  
    socklen_t client_addr_size = sizeof(client_addr);

    int client_fd = accept(listen_fd,                   // accept a connection from the open port from a client
                           client_addr,                 // produces a new file descriptor for socket created to communicate
                           &client_addr_size);          // with the client and fills in client addrss info
    assert(client_fd != -1);

    char address_str[INET6_ADDRSTRLEN];                 // fill in a string version of the client address for printing
    get_sock_address_string(client_addr, address_str);
    printf("SERVER: connection from %s\n", address_str);

    ret = write(client_fd, "Hello, world!", 13);
    assert(ret != -1);

    close(client_fd);                                   // done with client, release its socket
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

// Same as above but takes a socket address as an argument rather than a full addrinfo.
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
}
