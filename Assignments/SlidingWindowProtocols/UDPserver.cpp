// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT 7000
#define MAXLINE 1024 

// Driver code 
int main() { 
  int sockfd; 
  char buffer[MAXLINE]; 
  char *hello = "Hello from server"; 
  struct sockaddr_in servaddr, cliaddr; 
  
  // Creating socket file descriptor 
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  } 
  
  memset(&servaddr, 0, sizeof(servaddr)); 
  memset(&cliaddr, 0, sizeof(cliaddr)); 
  
  // Filling server information
  // The given port on this computer
  servaddr.sin_family = AF_INET; // IPv4 
  servaddr.sin_addr.s_addr = INADDR_ANY; 
  servaddr.sin_port = htons(PORT); 
  
  // Bind the socket with the server address 
  if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
      perror("bind failed"); 
      exit(EXIT_FAILURE); 
    } 
  printf("bound to: %x: %d\n", servaddr.sin_addr.s_addr, servaddr.sin_port);

  while(1) {
    printf("Server ready\n"); 
    
    int n;
    unsigned int len = sizeof(cliaddr); 
    
    // Receive on the socket
    // Get the message sent and
    // get the client address that it was sent from
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
		 0, (struct sockaddr *) &cliaddr, &len); 
    buffer[n] = '\0'; // Make sure string is null terrminated
    printf("Received from client: %s\n", buffer); 
    
    printf("from: %x: %x\n", cliaddr.sin_addr.s_addr, cliaddr.sin_port);
    
    sendto(sockfd, (const char *)hello, strlen(hello), 
	   0, (struct sockaddr *) &cliaddr, len); // len is sizeof(cliaddr) from above
    printf("Hello message sent to client.\n"); 
  }

  return 0; 
} 
