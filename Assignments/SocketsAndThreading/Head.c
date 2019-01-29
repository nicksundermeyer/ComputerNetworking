#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "duSocket.h"
#include <omp.h>

int main() {

  int connections = 3;
  int connected = 0;
  int sockets[connections];

  int sockfd = setupServerSocket(7000);

  // set it up to listen
  listen(sockfd,5);

  sockets[0] = serverSocketAccept(sockfd);

  while(connected < connections)
  {
    printf("start");
    sockets[connected] = serverSocketAccept(sockfd);
    connected++;
    printf("Accepted: %d/%d\n", connected, connections);
  }
  
  printf("All connected");

  for(int i=0; i<sizeof(sockets); i++)
  {
    char buffer[256];
    memset(&buffer, '\0', 256);
    //  bzero(buffer,256);

    int n = read(sockets[i],buffer,255);
    printf("%d\n", n);
    if (n < 0) {
      printf("ERROR reading from socket\n");
      exit(1);
    }

    printf("Here is the message: %s\n",buffer);
    n = write(sockets[i],"I got your message",18);
    if (n < 0) {
      printf("ERROR writing to socket\n");
      exit(1);
    }
  }

  // closing all sockets
  for(int i=0; i<sizeof(sockets); i++)
  {
    close(sockets[i]);
  }

  close(sockfd);
  return 0;
}
