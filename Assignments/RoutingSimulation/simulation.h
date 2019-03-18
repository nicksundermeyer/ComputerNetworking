
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
unsigned char* makePacket(unsigned char* data);

#endif /* simulation_h */
