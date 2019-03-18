#include "simulation.h"

#define PORT 7000
#define MAXLINE 10
int NUMROUTERS;

int main(int argc, char** argv) {
    if(argc>1)
    {
        NUMROUTERS = atoi(argv[1]);
    }

    omp_set_num_threads(NUMROUTERS);
#pragma omp parallel
    {
        char routername[78];
        sprintf(routername, "Router_%d", omp_get_thread_num());
        createRouter(routername);
    }
    return 0;
}

void createRouter(char* router_name) {
    int sockfd; 
    unsigned char buffer[MAXLINE]; 
    struct sockaddr_in servaddr;

    // setup sockets
    
    // Creating socket file descriptor
    // Last parameter could be IPPROTO_UDP but that is what it will pick anyway with 0
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&servaddr, 0, sizeof(servaddr));   // dest, src, size
    
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT+omp_get_thread_num()); 

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
    unsigned char* packet = makePacket("Hello World");

    // Start sending packet over socket
    while(1)
    {
        sendto(sockfd, (const char *)packet, MAXLINE, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
    }
}

// helper function to make a packet
unsigned char* makePacket(unsigned char* data) {
    unsigned char* result = (char*)malloc(MAXLINE);
    uint8_t type = 1;

    // copy packet type and data into packet
    memcpy(result, &type, sizeof(type));
    memcpy(result+sizeof(type), data, strlen(data));

    return(result);
}