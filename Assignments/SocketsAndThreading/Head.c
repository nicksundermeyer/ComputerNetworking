#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "duSocket.h"
// #include <omp.h>

int main() {
  int start = 1000;
  int end = 10000;

  int connections = 3;
  int connected = 0;
  int sockets[connections];

  int sockfd = setupServerSocket(7000);

  // set it up to listen
  listen(sockfd,5);

  while(connected < connections)
  {
    printf("start");
    sockets[connected] = serverSocketAccept(sockfd);
    connected++;
    printf("Accepted: %d/%d\n", connected, connections);
  }

  int rangePerNode = (end - start) / connections;
  int result = 0;

  for(int i=0; i<connections; i++)
  {
    int buffer = 0;

    int startEnd[2] = {start+(i*rangePerNode), (start + ((i+1)*rangePerNode)-1)};
    int n = write(sockets[i], &startEnd, sizeof(startEnd));
    if(n < 0)
    {
      printf("ERROR writing startEnd to socket\n");
      exit(0);
    }

    n = read(sockets[i],&buffer,sizeof(int));
    if (n < 0) {
      printf("ERROR reading from socket\n");
      exit(1);
    }

    // printf("Value: %d\n",buffer);
    n = write(sockets[i],"I got your message",18);
    if (n < 0) {
      printf("ERROR writing to socket\n");
      exit(1);
    }

    result += buffer;
  }

  printf("Result: %d\n", result);

  // closing all sockets
  for(int i=0; i<sizeof(sockets); i++)
  {
    close(sockets[i]);
  }

  close(sockfd);
  return 0;
}
