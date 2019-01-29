#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {

  // Socket pointer
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    fprintf(stderr,"ERROR opening socket\n");
    exit(0);
  }

  // port number
  int portno = 7000;

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr)); 

  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

   // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig byte first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);


  // Setup the server host address
  struct hostent *server;
  server = gethostbyname("localhost");
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);  /// dest, src, size

  // Connect to the server
  if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR connecting\n");
    exit(0);
  }

  char buffer[256];
  printf("Please enter the message: ");
  memset(&buffer, '\0', 256); 
  fgets(buffer,255,stdin);

  int n = write(sockfd,buffer,strlen(buffer));
  if (n < 0) {
    printf("ERROR writing to socket\n");
    exit(0);
  }

  memset(&buffer, '\0', 256);
  n = read(sockfd,buffer,255);
  if (n < 0) {
    printf("ERROR reading from socket\n");
    exit(0);
  }
  printf("%s\n",buffer);

  close(sockfd);
  return 0;

}
