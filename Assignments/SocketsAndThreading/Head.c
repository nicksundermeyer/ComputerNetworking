#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>
#include "duSocket.h"

int main() {
  int start = 1000;
  int end = 1000000;

  int connections = 3;
  int connected = 0;
  int sockets[connections];

  int sockfd = setupServerSocket(7000);

  // set it up to listen
  listen(sockfd,5);

  // wait for all socket connections
  while(connected < connections)
  {
    sockets[connected] = serverSocketAccept(sockfd);
    connected++;
    printf("Accepted: %d/%d\n", connected, connections);
  }

  // split range among connections
  int rangePerNode = (end - start) / connections;
  int result = 0;
  time_t startTime = time(0);

  // write to sockets
  for(int i=0; i<connections; i++)
  {
    writeInt(start+(i*rangePerNode), sockets[i]);
    writeInt((start + ((i+1)*rangePerNode)-1), sockets[i]);
  }

  // read from sockets
  for(int i=0; i<connections; i++)
  {
    int buffer = readInt(sockets[i]);
    result += buffer;
  }
  
  time_t endTime = time(0);

  printf("Result: %d\n", result);
  printf("Time taken: %ld Seconds\n", (endTime-startTime));

  // closing all sockets
  for(int i=0; i<sizeof(sockets); i++)
  {
    close(sockets[i]);
  }

  close(sockfd);
  return 0;
}
