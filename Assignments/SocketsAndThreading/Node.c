#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <omp.h>
#include "duSocket.h"

void main() {
#pragma omp parallel
    {
        printf("creating thread\n");

        int sockfd = callServer("localhost", 7000);

        int result = 0;

        int start = readInt(sockfd);
        int end = readInt(sockfd);

        int thread = omp_get_thread_num();
        printf("Starting calculation from %d to %d on thread %d\n", start, end, thread);

        for (int i = start; i <= end; i++) {
            int prime = 1; // start by assuming the current number is prime
            for (int j = 2; j < i; j++) { // Loop till j < i
                if (i % j == 0) {
                    prime = 0;
                    break;
                }
            }
            if (prime == 1) {
                result += 1; // Add to result
            }
        }

        printf("Result: %d\n", result);

        writeInt(result, sockfd);

        memset(&result, '\0', sizeof(int));

        close(sockfd);
    }
    return 0;
}