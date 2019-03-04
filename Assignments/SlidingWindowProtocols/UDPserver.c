// Server side implementation of UDP client-server model
#include "UDPserver.h"

#define PORT 7000
#define MAXLINE 24
#define PERPACKET 20

// Driver code 
int main() {

  int sockfd;
  uint16_t expectedseqnum = 0;
  uint16_t lastACK = 0;
  unsigned char* buffer = (char*)malloc(MAXLINE);
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

  FILE *fp = fopen("Desktop/ComputerNetworking/Assignments/SlidingWindowProtocols/fileOut", "wb");
//    FILE *fp = fopen("fileOut", "wb");

  // loop to continue waiting for packets
  while(1) {
    printf("Server ready\n"); 
    
    int n;
    unsigned int len = sizeof(cliaddr); 
    
    // Receive on the socket
    // Get the message sent and
    // get the client address that it was sent from
    n = recvfrom(sockfd, (unsigned char *)buffer, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);

    char buf[MAXLINE];
    memcpy(&buf, buffer, MAXLINE);
      
    // buf[n] = '\0'; // Make sure string is null terminated
    // printf("Received from client: %s\n", buf);
    // printf("from: %x: %x\n", cliaddr.sin_addr.s_addr, cliaddr.sin_port);
      
    if(buf[0] == '-' && buf[1] == '-' && buf[2] == '-')
    {
      sendto(sockfd, (const char *)("1"), 1, 0, (struct sockaddr *) &cliaddr, len);
      break;
    }

    // copy information out of buffer
    uint16_t seq;
    memcpy(&seq, buffer, 2);

    uint16_t checkSum;
    memcpy(&checkSum, buffer+sizeof(seq), 2);

    unsigned char * data = (char*)malloc(PERPACKET);
    memcpy(data, buffer+4, PERPACKET); 

    // verify checksum is correct
    int correct = verifyChecksum(checkSum, data);

    printf("%d/%d\n", seq, expectedseqnum);
      
    if(correct == 1 && seq == expectedseqnum)
    {
      printf("Checksum verified!\n");

      // copy sequence number into char* to send ack
      const char* response = (char*)malloc(PERPACKET);
      memcpy(response, &seq, 2);

      // sending ack
      sendto(sockfd, (const char *)response, 2,
      0, (struct sockaddr *) &cliaddr, len); // len is sizeof(cliaddr) from above
      printf("ACK sent to client\n"); 

      // increment expected sequence number
      expectedseqnum += PERPACKET;

      // set last acked sequence number
      lastACK = seq;

      // write data to file
      fputs(data, fp);
    }
    else {
      printf("Incorrect packet: %d/%d\n", correct == 1, seq == expectedseqnum);
        printf("expected packet: %d\n", expectedseqnum);
        printf("actual packet: %d\n", seq);
        
      // ACK last successful sequence number
      const char* response = (char*)malloc(PERPACKET);
      memcpy(response, &lastACK, 2);

      // sending ack
      sendto(sockfd, (const char *)response, 1, 0, (struct sockaddr *) &cliaddr, len); // len is sizeof(cliaddr) from above
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

void print_bits ( void* buf, size_t size_in_bytes )
{
    char* ptr = (char*)buf;

    for (size_t i = 0; i < size_in_bytes; i++) {
        for (short j = 7; j >= 0; j--) {
            printf("%d", (ptr[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}
