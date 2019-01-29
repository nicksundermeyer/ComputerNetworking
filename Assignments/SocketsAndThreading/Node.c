#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "duSocket.h"

int main() {

  int sockfd = callServer("localhost", 7000);

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
