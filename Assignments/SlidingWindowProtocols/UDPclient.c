#include "UDPclient.h"

#define PORT 7000
#define MAXLINE 1024
#define PERPACKET 20

// Driver code 
int main() { 
  int sockfd; 
  unsigned char buffer[MAXLINE]; 
  unsigned char *data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec facilisis sollicitudin mauris dignissim viverra. Phasellus euismod tellus sit amet arcu gravida, sit amet bibendum felis fringilla. Pellentesque magna velit, vulputate et pellentesque quis, laoreet non ante. Etiam mollis tempor ultrices. Proin tempus volutpat justo, vel scelerisque ligula gravida vitae. Etiam purus ipsum, venenatis vel dui sed, dapibus pharetra est. Aliquam ac ipsum in neque porttitor rhoncus a non metus. Etiam sit amet suscipit turpis, ut pretium purus. Proin euismod volutpat orci eu ultricies. Vestibulum vel diam urna. Sed placerat id enim et ultricies. Integer at vulputate velit. Vivamus mattis bibendum libero, a luctus orci dapibus sit amet. Phasellus accumsan gravida purus. Quisque sit amet efficitur eros.";
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
  unsigned char* host = "localhost";
  //  char* host="linux2.cs.du.edu";

  struct hostent *server;
  server = gethostbyname(host);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  memcpy(&servaddr.sin_addr.s_addr, server->h_addr, server->h_length);
  
  int n;
  unsigned int len = sizeof(servaddr); 
  
//  // 4th parameter is for flags - 0 is fine for these examples
//  sendto(sockfd, (const char *)data, strlen(data),
//  0, (struct sockaddr *) &servaddr, sizeof(servaddr));
//  printf("Hello message sent to server.\n");
//  printf("%x: %x\n", servaddr.sin_addr.s_addr, servaddr.sin_port);
//
//  // Note that I could put NULL for the last 2 arguments if I know exactly
//  //   what server is sending me info (which in theory I should know)
//  n = recvfrom(sockfd, (char *)buffer, MAXLINE,
//            0, (struct sockaddr *) &servaddr, &len);
//  buffer[n] = '\0';  // Just making sure the string sent is null terminated
//  printf("Received from server: %s\n", buffer);
//  printf("from: %x: %x\n", servaddr.sin_addr.s_addr, servaddr.sin_port);

    int sentComplete = 0;
    unsigned char* sequenceNum = "0";
    int sequenceNumInt = 0;
    unsigned char dataToSend[PERPACKET+1];
    while (!sentComplete) {
        // Create packet
        sprintf(dataToSend, "%.*s", PERPACKET, data + sequenceNumInt);
        printf("Data going into packet: %s\n", dataToSend);
        unsigned char* newPacket = makePacket(sequenceNum, dataToSend);
        printf("Packet: %s\n", newPacket);
        // Send packet
        sendto(sockfd, (unsigned char *)newPacket, strlen(newPacket), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        // Wait for ACK, timeout if no response
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 1000000; // waits 1sec
        
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *)&servaddr, &len);
        
        // No response, send again
        
        // Find next section of data to send for packet
        sequenceNum += 24;
        // Send next packet
        
        // Done sending, exit loop
        sentComplete = 1;
    }
    
    
  close(sockfd); 
  return 0; 
} 

unsigned char* makePacket(unsigned char* seq, unsigned char* data) {
  uint16_t checksum = checkSum(data);
  unsigned char result[24];

  memcpy(result, seq, strlen(seq));
  memcpy(result+strlen(seq), &checksum, sizeof(checksum));
  memcpy(result+sizeof(checksum)+strlen(seq), data, strlen(data));

  unsigned char* ret = result;
  return(ret);
}

uint16_t checkSum(unsigned char* data) {
  uint32_t sum = 0;

  // add to checksum
  for(int i = 0; i < strlen(data); i++)
  {
    sum += data[i];

    // deal with overflow
    sum += sum >> 16;
  }

  // checksum =~ checksum;
  sum =~ sum;
  return sum;
}

void print_char(char x)
{
    for (int i = 0; i < 8; i++) {
        printf("%d", (x & 0x8000) >> 15);
        x <<= 1;
    }
    printf("\n");
}

void print_int(uint16_t x)
{
    for (int i = 0; i < 16; i++) {
        printf("%d", (x & 0x8000) >> 15);
        x <<= 1;
    }
    printf("\n");
}
