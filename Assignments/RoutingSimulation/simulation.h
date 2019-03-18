
#ifndef simulation_h
#define simulation_h

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <omp.h>

void createRouter(char* router_name, int router_num);
void print_bits ( void* buf, size_t size_in_bytes );

#endif /* simulation_h */
