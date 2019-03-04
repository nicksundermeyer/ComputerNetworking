#include "UDPclient.h"

#define PORT 7000
#define MAXLINE 24
#define PERPACKET 20
#define WINDOWSIZE 1

// Driver code 
int main() {
  int sockfd; 
  unsigned char buffer[MAXLINE]; 
  struct sockaddr_in servaddr;

  // reading data from file
  FILE *fIn;
//    if ((fIn = fopen("fileIn", "rb")) == NULL) {
    if ((fIn = fopen("fileIn", "rb")) == NULL) {
        printf("Error reading file!\n");
        exit(1);
    }
    
  // find size of file
  fseek(fIn, 0L, SEEK_END);
  long fSize = ftell(fIn);
  rewind(fIn);

  // read bytes in from file
  unsigned char data[fSize];
  size_t read_bytes = fread(data, fSize, 1, fIn);
    // print_data(data, fSize);
    
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
    
  uint16_t windowStart = 0;
  unsigned char *dataToSend = (char *) malloc(PERPACKET);

  while (windowStart < fSize) {
      int t = fSize-PERPACKET;

      // select data for packet
      sprintf(dataToSend, "%.*s", PERPACKET, data+windowStart);

      //create packet
      unsigned char* packet = makePacket(windowStart, dataToSend);

      // send packet over socket
      sendto(sockfd, (const char *)packet, MAXLINE, 
      0, (struct sockaddr *) &servaddr, sizeof(servaddr)); 

      // Wait for ACK, timeout if no response
      struct timeval timeout = {1, 0};
      setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
      
      n = recvfrom(sockfd, (char *)buffer, 1, 0, (struct sockaddr *)&servaddr, &len);
      if (n >= 0) {
          //check response
          uint16_t response;
          memcpy(&response, buffer, 2);
          
          printf("Received ACK! %d\n", response);

          // set window to start at the location in the data for the ACKed packet
          if(response > 0) {
            windowStart = response + PERPACKET;
          }
      }
  }

  const char * packet = "---";
  sendto(sockfd, (const char *)packet, strlen(packet), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
  n = recvfrom(sockfd, (char *)buffer, 1, 0, (struct sockaddr *)&servaddr, &len);

  close(sockfd); 
  return 0; 
} 

unsigned char* makePacket(uint16_t seq, unsigned char* data) {
  uint16_t checksum = checkSum(data);
  unsigned char* result = (char*)malloc(MAXLINE);

  memcpy(result, &seq, sizeof(seq));
  memcpy(result+sizeof(seq), &checksum, sizeof(checksum));
  memcpy(result+sizeof(checksum)+sizeof(seq), data, strlen(data));

  return(result);
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

  // conver to uint16_t and invert bits
  uint16_t checksum = sum;
  checksum =~ checksum;
  return checksum;
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

void print_data(void* buf, size_t size_in_bytes) {
    int i;
    printf("PRINTING DATA:\n");
    for(i = 0; i < size_in_bytes; ++i)
        printf("%c", ((char *)buf)[i]);
    printf("\nEND OF DATA\n");
}
