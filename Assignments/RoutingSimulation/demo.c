#include "demo.h"

#define PORT 7000
#define MAXLINE 20

int main() {
    while(1)
    {
        int src, dest;
        printf("Enter the source router: ");
        scanf("%d", &src);
        printf("Enter the destination router: ");
        scanf("%d", &dest);


        // set up port and send
        int sockfd; 
        unsigned char buffer[MAXLINE]; 
        struct sockaddr_in servaddr;

        // setup socket
        
        // Creating socket file descriptor
        // Last parameter could be IPPROTO_UDP but that is what it will pick anyway with 0
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
            perror("socket creation failed"); 
            exit(EXIT_FAILURE); 
        } 

        memset(&servaddr, 0, sizeof(servaddr));   // dest, src, size
        
        // Filling server information 
        servaddr.sin_family = AF_INET; 
        servaddr.sin_port = htons(PORT+src); 

        // Setup the server host address
        unsigned char* host = "localhost";

        struct hostent *server;
        server = gethostbyname(host);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            exit(0);
        }
        memcpy(&servaddr.sin_addr.s_addr, server->h_addr, server->h_length);

        //Make packet to send
        unsigned char* packet = makePacket(src, dest, "Hello World");

        // Send packet over socket
        sendto(sockfd, (const char *)packet, MAXLINE, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
    }
}

// helper function to make a packet
unsigned char* makePacket(uint16_t src, uint16_t dest, unsigned char* data) {
    uint8_t type = 1;
    unsigned char* result = (char*)malloc(MAXLINE);

    // copy packet type and data into packet
    memcpy(result, &type, sizeof(type));
    memcpy(result+sizeof(type), &src, sizeof(src));
    memcpy(result+sizeof(type)+sizeof(src), &dest, sizeof(dest));
    memcpy(result+sizeof(type)+sizeof(src)+sizeof(dest), data, strlen(data));

    return(result);
}

// helper function to print out bits
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
