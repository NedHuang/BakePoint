// simple_client.c: contact server and receive a "hello world"
// Adapted from http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#syscalls
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>

#define PORT "12344"            // the port client will be connecting to 

#define MAXDATASIZE 100         // max number of bytes we can get at once 

void get_address_string(struct addrinfo *addr, char buffer[]);
void get_sock_address_string(struct sockaddr *addr, char buffer[]);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }
  char *hostname = argv[1];
  int ret;

  struct addrinfo *serv_addr;                         // translate the hostname specified to binary
  ret = getaddrinfo(hostname, PORT,                   // and look up its internet address using DNS
                    NULL, &serv_addr);
  assert(ret == 0);

  // Note: there may be several addresses in the linked list assocated
  // serv_addr, some of which work and others of which don't. The
  // remaining code assumes the first one pointed to directly by
  // serv_addr will work. More robust code should cycle through the
  // list tring to connect() to each one until successful.

  int sockfd = socket(serv_addr->ai_family,           // create a socket with the appropriate params
                      serv_addr->ai_socktype,         // to the server
                      serv_addr->ai_protocol);
  assert(ret != -1);

  ret = connect(sockfd,                               // connect the socket to the server so that 
                serv_addr->ai_addr,                   // writes will send over the network
                serv_addr->ai_addrlen);
  assert(ret != -1);


  char address_str[INET6_ADDRSTRLEN];                 // fill in a string version of the addrss which was resolved
  get_address_string(serv_addr, address_str);         // defined below, fills in buffer with printable address
  printf("client: connected to %s\n", address_str);

  freeaddrinfo(serv_addr);                            // done with structure, free it up

  char buf[MAXDATASIZE];
  int nbytes = read(sockfd, buf, MAXDATASIZE-1);      // receive data from the server
  assert(nbytes != -1);
  buf[nbytes] = '\0';
  printf("client: received '%s'\n",buf);

  // int numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0); // alternative to read() is recv() with additional options 

  close(sockfd);

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
  get_sock_address_string((struct sockaddr *)addr->ai_addr, buffer);

  // void *ip_address = NULL;
  // if(addr->ai_family == AF_INET){               // ipv4 address
  //   ip_address = (void *) &((struct sockaddr_in*)addr->ai_addr)->sin_addr;
  // }
  // else if(addr->ai_family == AF_INET6){         // ipv6 address
  //   ip_address = (void *) &((struct sockaddr_in6*)addr->ai_addr)->sin6_addr;
  // }
  // inet_ntop(addr->ai_family, ip_address, buffer, INET6_ADDRSTRLEN);
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
