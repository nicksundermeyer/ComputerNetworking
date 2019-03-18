#include "simulation.h"

#define PORT 7000
#define MAXLINE 20
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
    // create table for BF & fill with -1
    int table[NUMROUTERS][NUMROUTERS];
    for (int i = 0; i < NUMROUTERS; i++) {
        for (int j = 0; j < NUMROUTERS; j++) {
            table[i][j] = -1;
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
    int neighbors[fSize];
    size_t read_data = fread(neighbors, 1, fSize, fIn);
    printf("Created router %d with %d neighbors.\n", router_num, fSize);
    
    // fill initial row of table
    for (int i = 0; i < NUMROUTERS; i++) {
        table[router_num][i] = neighbors[i];
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

    // loop to wait for packets
    while(1)
    {
        printf("Server ready\n");

        unsigned int len = sizeof(cliaddr);
        int n = recvfrom(sockfd, (unsigned char *)buffer, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);

        char buf[MAXLINE];
        memcpy(&buf, buffer, MAXLINE);

        uint8_t type;
        memcpy(&type, buffer, sizeof(type));
        printf("Type: %d\n", type);

        uint16_t src;
        memcpy(&src, buffer+sizeof(type), sizeof(src));
        printf("Src: %d\n", src);

        uint16_t dest;
        memcpy(&dest, buffer+sizeof(type)+sizeof(src), sizeof(dest));
        printf("Dest: %d\n", dest);

        // print_bits(buf, MAXLINE);
    }
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
