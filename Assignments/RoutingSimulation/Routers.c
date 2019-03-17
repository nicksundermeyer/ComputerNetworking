
#include "Routers.h"

#define NUMROUTERS 3

int main() {
    omp_set_num_threads(NUMROUTERS);
#pragma omp parallel
    {
//        char filename[16];
//        sprintf(filename, "neighbors-%d.txt", omp_get_thread_num());
        char filename[78];
        sprintf(filename, "Desktop/ComputerNetworking/Assignments/SlidingWindowProtocols/neighbors-%d.txt", omp_get_thread_num());
        createRouter(filename);
    }
    return 0;
}

void createRouter(char* router_name) {
    printf("Creating thread for: %s\n", router_name);
    FILE *fIn = fopen(router_name, "wb");
    if (fIn == NULL) {
        printf("Error readig in file %s.\n", router_name);
        exit(1);
    }
    
    
}

