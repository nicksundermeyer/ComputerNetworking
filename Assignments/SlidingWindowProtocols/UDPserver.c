// Server side implementation of UDP client-server model 
#include "UDPserver.h"

#define PORT 7000
#define MAXLINE 23 

// Driver code 
int main() { 
  int sockfd; 
  char buffer[MAXLINE]; 
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

  FILE *fp = fopen("fileOut", "w");

  // loop to continue waiting for packets
  while(1) {
    printf("Server ready\n"); 
    
    int n;
    unsigned int len = sizeof(cliaddr); 
    
    // Receive on the socket
    // Get the message sent and
    // get the client address that it was sent from
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);

    if(buffer[0] == '\0')
    {
      break;
    }

    buffer[n] = '\0'; // Make sure string is null terminated
    printf("Received from client: %s\n", buffer);
    // printf("from: %x: %x\n", cliaddr.sin_addr.s_addr, cliaddr.sin_port);

    // copy information out of buffer
    char seq = buffer[0];

    uint16_t checkSum;
    memcpy(&checkSum, buffer+1, 2);

    char * data;
    memcpy(data, buffer+3, 21);

    // verify checksum is correct
    int correct = verifyChecksum(checkSum, data);

    if(correct == 1)
    {
      printf("Checksum verified!\n");

      // sending ack
      sendto(sockfd, (const char *)("1"), 1, 
      0, (struct sockaddr *) &cliaddr, len); // len is sizeof(cliaddr) from above
      printf("ACK sent to client\n"); 

      // write data to file
      fputs(data, fp);
    }
    else {
      printf("Checksum incorrect\n");
    }
  }

  fclose(fp);

  return 0; 
} 

int verifyChecksum(uint16_t testSum, unsigned char* data) {
  uint32_t sum = 0;

  // add to checksum
  for(int i = 0; i < strlen(data); i++)
  {
    sum += data[i];

    // deal with overflow
    sum += sum >> 16;
  }

  // conver to uint16_t and invert bits
  uint16_t checksum = sum;
  checksum =~ checksum;

  // compare calculated checksum with incoming checksum
  if(checksum == testSum)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
