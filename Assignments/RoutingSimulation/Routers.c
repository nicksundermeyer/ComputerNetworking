
#include "Routers.h"

#define NUMROUTERS 3

int main() {
#pragma omp parallel for schedule(static, 2) reduction(+:temp)
    for (int i = 0; i < NUMROUTERS; i++) {
        createRouter();
    }
    return 0;
}

void createRouter() {
    
}

