#include "simulation.h"

#define PORT 7000
#define MAXLINE 10
int NUMROUTERS;
char* ROOTNAME;

int main(int argc, char** argv) {
    if(argc>1)
    {
        NUMROUTERS = atoi(argv[1]);
        ROOTNAME = argv[2];
    }

    omp_set_num_threads(NUMROUTERS);
#pragma omp parallel
    {
        char routername[16];
        sprintf(routername, "Router_%d", omp_get_thread_num());
        createRouter(routername, omp_get_thread_num());
    }
    return 0;
}

void createRouter(char* router_name, int router_num) {
    int sockfd; 
    unsigned char buffer[MAXLINE]; 
    struct sockaddr_in servaddr;
    
    // read file
    char router_file[100];
//    sprintf(router_file, "Desktop/ComputerNetworking/Assignments/RoutingSimulation/%s-%d.txt", ROOTNAME, omp_get_thread_num());
    sprintf(router_file, "%s-%d.txt", ROOTNAME, omp_get_thread_num());
    FILE *fIn = fopen(router_file, "rb");
    if (fIn == NULL) {
        printf("ERROR: Unable to open file [%s].\n", router_file);
        exit(1);
    }
    
    fseek(fIn, 0L, SEEK_END);
    long fSize = ftell(fIn);
    rewind(fIn);
    if (fSize != NUMROUTERS) {
        printf("ERROR: Incorrect file format for router configuration.\n");
        exit(1);
    }
    int neighbors[fSize];
    size_t read_data = fread(neighbors, 1, fSize, fIn);
    printf("Created router %d with %d neighbors.\n", router_num, fSize);
    
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
    servaddr.sin_port = htons(PORT+router_num);

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
