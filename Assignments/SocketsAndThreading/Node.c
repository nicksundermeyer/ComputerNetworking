#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {


  // Get a socket of the right type
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("ERROR opening socket");
    exit(1);
  }

  // port number
  int portno = 7000;

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr)); 

  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

  // Basically the machine we are on...
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig bype first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);

  // Bind the socket to the given port
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR on binding\n");
    exit(1);
  }

  // set it up to listen
  listen(sockfd,5);
  

  int newsockfd;
  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(cli_addr);

  // Wait for a call
  printf("waiting for a call...\n");
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  printf("connected\n");
  if (newsockfd < 0) {
    printf("ERROR on accept");
    exit(1);
  }

  char buffer[256];
  memset(&buffer, '\0', 256);
  //  bzero(buffer,256);
  int n = read(newsockfd,buffer,255);
  if (n < 0) {
    printf("ERROR reading from socket\n");
    exit(1);
  }

  printf("Here is the message: %s\n",buffer);
  n = write(newsockfd,"I got your message",18);
  if (n < 0) {
    printf("ERROR writing to socket\n");
    exit(1);
  }

  close(newsockfd);
  close(sockfd);
  return 0;
}
