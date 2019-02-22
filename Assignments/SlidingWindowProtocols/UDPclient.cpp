// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h> // gethostbyname - IPv4


#define PORT 7000
#define MAXLINE 1024 

// Driver code 
int main() { 
  int sockfd; 
  char buffer[MAXLINE]; 
  char *data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec facilisis sollicitudin mauris dignissim viverra. Phasellus euismod tellus sit amet arcu gravida, sit amet bibendum felis fringilla. Pellentesque magna velit, vulputate et pellentesque quis, laoreet non ante. Etiam mollis tempor ultrices. Proin tempus volutpat justo, vel scelerisque ligula gravida vitae. Etiam purus ipsum, venenatis vel dui sed, dapibus pharetra est. Aliquam ac ipsum in neque porttitor rhoncus a non metus. Etiam sit amet suscipit turpis, ut pretium purus. Proin euismod volutpat orci eu ultricies. Vestibulum vel diam urna. Sed placerat id enim et ultricies. Integer at vulputate velit. Vivamus mattis bibendum libero, a luctus orci dapibus sit amet. Phasellus accumsan gravida purus. Quisque sit amet efficitur eros.";
  struct sockaddr_in servaddr; 

  // Creating socket file descriptor
  // Last parameter could be IPPROTO_UDP but that is what it will pick anyway with 0
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  } 

  memset(&servaddr, 0, sizeof(servaddr));   // dest, src, size
  
  // Filling server information 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(PORT); 

  // Setup the server host address
  char* host="localhost";
  //  char* host="linux2.cs.du.edu";

  struct hostent *server;
  server = gethostbyname("localhost");
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  memcpy(&servaddr.sin_addr.s_addr, server->h_addr, server->h_length);
  
  int n;
  unsigned int len = sizeof(servaddr); 
  
  // 4th parameter is for flags - 0 is fine for these examples
  sendto(sockfd, (const char *)data, strlen(data), 
	 0, (struct sockaddr *) &servaddr, sizeof(servaddr)); 
  printf("Hello message sent to server.\n"); 
  printf("%x: %x\n", servaddr.sin_addr.s_addr, servaddr.sin_port);

  // Note that I could put NULL for the last 2 arguments if I know exactly
  //   what server is sending me info (which in theory I should know)
  n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
    	       0, (struct sockaddr *) &servaddr, &len); 
  buffer[n] = '\0';  // Just making sure the string sent is null terminated
  printf("Received from server: %s\n", buffer); 
  printf("from: %x: %x\n", servaddr.sin_addr.s_addr, servaddr.sin_port);

  close(sockfd); 
  return 0; 
} 

char* makePacket(char* data) {

}