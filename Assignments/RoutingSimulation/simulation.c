
#include "simulation.h"

#define NUMROUTERS 3

int main() {
    omp_set_num_threads(NUMROUTERS);
#pragma omp parallel
    {
//        char filename[16];
//        sprintf(filename, "neighbors-%d.txt", omp_get_thread_num());
        char filename[78];
        sprintf(filename, "Desktop/ComputerNetworking/Assignments/RoutingSimulation/neighbors-%d.txt", omp_get_thread_num());
        createRouter(filename);
    }
    return 0;
}

void createRouter(char* router_name) {
    printf("Creating thread for: %s\n", router_name);
    FILE *fIn = fopen(router_name, "rb");
    if (fIn == NULL) {
        printf("Error readig in file %s.\n", router_name);
        exit(1);
    }
    
    int neighbors[NUMROUTERS];
    fseek(fIn, 0L, SEEK_END);
    long fSize = ftell(fIn);
    if (fSize != NUMROUTERS) {
        printf("Error with incorrect router configuration.\n");
        exit(1);
    }
    rewind(fIn);
    
    size_t read_data = fread(neighbors, 1, fSize, fIn);
    printf("Router neighbors: %d %d %d\n", neighbors[0], neighbors[1], neighbors[2]);
}

