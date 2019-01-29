#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "duSocket.h"

int main() {

  int sockfd = setupServerSocket(7000);

  // set it up to listen
  listen(sockfd,5);

  int newsockfd = serverSocketAccept(sockfd);

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
