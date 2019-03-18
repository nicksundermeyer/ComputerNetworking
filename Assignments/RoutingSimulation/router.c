#include "router.h"

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
    
    return 0;
}


