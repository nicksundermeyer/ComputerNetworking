#include "router.h"

#define PORT 7000

int main() {
    int router_num;
    char filename[100];
    printf("Enter the router number: ");
    scanf("%d", &router_num);
    printf("Enter the root name: ");
    scanf("%s", &filename);
    printf("Creating router %d...\n", router_num);
    
    char router_file[100];
    sprintf(router_file, "Desktop/ComputerNetworking/Assignments/RoutingSimulation/%s-%d.txt", filename, router_num);
    FILE *fIn = fopen(router_file, "rb");
    if (fIn == NULL) {
        printf("ERROR: Unable to open file [%s].\n", router_file);
        exit(1);
    }
    
    fseek(fIn, 0L, SEEK_END);
    long fSize = ftell(fIn);
    rewind(fIn);
    int neighbors[fSize];
    size_t read_data = fread(neighbors, 1, fSize, fIn);
    printf("Created router with %d neighbors.\n", fSize);
    
    startListening(router_num, neighbors);
    
    return 0;
}

void startListening(int router_number, int* list_of_neighbors) {
    int sockfd;
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));   // dest, src, size
    
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT+router_number);
    
    // Setup the server host address
    unsigned char* host = "localhost";
    
    struct hostent *server;
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR: No such host.\n");
        exit(1);
    }
    memcpy(&servaddr.sin_addr.s_addr, server->h_addr, server->h_length);
    printf("did something!\n");
}


