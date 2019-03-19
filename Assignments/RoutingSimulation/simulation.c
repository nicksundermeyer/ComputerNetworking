#include "simulation.h"

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
    // create table for BF & fill with -1
    uint8_t table[NUMROUTERS][NUMROUTERS];
    for (int i = 0; i < NUMROUTERS; i++) {
        for (int j = 0; j < NUMROUTERS; j++) {
            table[i][j] = 255;
        }
    }
    
    // read file
    char router_file[100];
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
    char* neighbors = (char*) malloc(fSize);
    size_t read_data = fread(neighbors, 1, fSize, fIn);
    printf("Created router %d with %d neighbors.\n", router_num, fSize);
    
    // fill initial row of table
    for (int i = 0; i < NUMROUTERS; i++) {
        table[router_num][i] = neighbors[i] - '0';
    }
    printf("Two Dimensional array elements:\n");
    for(int i=0; i<3; i++) {
        for(int j=0;j<3;j++) {
            printf("(%d, %d) ", router_num, table[i][j]);
            if(j==2){
                printf("\n");
            }
        }
    }
    
    // setup socket
    int sockfd;
    
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
    servaddr.sin_port = htons(PORT+omp_get_thread_num());
    
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("bound to: %x: %d\n", servaddr.sin_addr.s_addr, servaddr.sin_port);
    
    sendPacketToNeighbors(router_num, table);
    
    // loop to wait for packets
    while(1)
    {
        printf("Router ready\n");
        
        unsigned int len = sizeof(cliaddr);
        int n = recvfrom(sockfd, (unsigned char *)buffer, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);
        
        char buf[MAXLINE];
        memcpy(&buf, buffer, MAXLINE);
        
        uint8_t type;
        memcpy(&type, buffer, sizeof(type));
        
        if (type == 0) {
            printf("Type: Controlled packet\n", type);

            uint8_t temp_table[NUMROUTERS][NUMROUTERS];
            memcpy(&temp_table, buffer+1, NUMROUTERS*NUMROUTERS);

            bool checkEqual = true;
            for (int i = 0; i < NUMROUTERS; i++) {
                for (int j = 0; j < NUMROUTERS; j++) {
                    if (table[i][j] != temp_table[i][j]) {
                        checkEqual = false;
                    }
                }
            }
            
            if (!checkEqual) { // Send packets until all tables at each router are equal
                sendPacketToNeighbors(router_num, table);
            }
            else {
                int minValue = 255;
                for (int i = 0; i < NUMROUTERS; i++) {
                    if (table[router_num][i] != 0 && table[router_num][i] != 255) {
                        // for each neighbor
                            // if the distance to that neighbor plus its distance to destination is less than table[router_num][i]...
                        for (int j = 0; j < NUMROUTERS; j++) {
                            if (table[router_num][j] != 0 && table[router_num][j] != 255) {
                                if (table[router_num][i] > table[i][j] + table[j][i]) {
                                    minValue = table[i][j] + table[j][i];
                                    table[i][j] = minValue;
                                }
                            }
                        }
                    }
                }
                sendPacketToNeighbors(router_num, table);
            }
        }
        else if (type == 1) {
            printf("Type: Basic packet\n", type);
            
            uint16_t src;
            memcpy(&src, buffer+sizeof(type), sizeof(src));
            printf("Src: %d\n", src);
            
            uint16_t dest;
            memcpy(&dest, buffer+sizeof(type)+sizeof(src), sizeof(dest));
            printf("Dest: %d\n", dest);
        }
        else {
            printf("Invalid packet type\n");
        }
        
        // print_bits(buf, MAXLINE);
    }
}

// helper function to make control packet
unsigned char* makeControlPacket(uint8_t data[NUMROUTERS][NUMROUTERS]) {
    uint8_t type = 0;
    size_t s = 1+(NUMROUTERS*NUMROUTERS);
    unsigned char* result = (char*)malloc(s);

    // copy packet type and data into packet
    memcpy(result, &type, sizeof(type));
    memcpy(result+1, &data, NUMROUTERS*NUMROUTERS);

    return(result);
}

// helper function to send data to socket
void sendToSocket(uint8_t dest, unsigned char* packet)
{
    // set up port and send
        int sockfd; 
        unsigned char buffer[1+(NUMROUTERS*NUMROUTERS)]; 
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
        servaddr.sin_port = htons(PORT+dest); 

        // Setup the server host address
        unsigned char* host = "localhost";

        struct hostent *server;
        server = gethostbyname(host);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            exit(0);
        }
        memcpy(&servaddr.sin_addr.s_addr, server->h_addr, server->h_length);

        // Send packet over socket
        sendto(sockfd, (const char *)packet, 1+(NUMROUTERS*NUMROUTERS), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
}

void sendPacketToNeighbors(int router_num, uint8_t table[NUMROUTERS][NUMROUTERS]) {
    for (int i = 0; i < NUMROUTERS; i++) {
        if (table[router_num][i] != 0 && table[router_num][i] != 255) {
            // Send out packet
            uint8_t dest = i;
            unsigned char* packet = makeControlPacket(table);
            sendToSocket(dest, packet);
        }
    }
}

// helper function to print packet out as uint8_ts
void print_numbers(char* buf, size_t size_in_bytes)
{
    for(int i=0; i<size_in_bytes; i++)
    {
        printf("%d ", buf[i]);
    }
    printf("\n");
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
