#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include "duSocket.h"

int main() {

  int sockfd = callServer("localhost", 7000);

  int result = 0;
  int start = 1000;
  int end = 10000;

  for (int i = start; i <= end; i++) {
    int prime = 1; // start by assuming the current number is prime
    for (int j = 2; j < i; j++) { // Loop till j < i
        if (i % j == 0) {
            prime = 0;
            break;
        }
    }
    if (prime == 1) {
        result += 1; // Add to result
    }
  }

  printf("Result: %d\n", result);

  int n = write(sockfd,&result,sizeof(int));
  if (n < 0) {
    printf("ERROR writing to socket\n");
    exit(0);
  }

  memset(&result, '\0', sizeof(int));
  n = read(sockfd,result,255);
  if (n < 0) {
    printf("ERROR reading from socket\n");
    exit(0);
  }
  // printf("%d\n",result);

  close(sockfd);
  return 0;

}
