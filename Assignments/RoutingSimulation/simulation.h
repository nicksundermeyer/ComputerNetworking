
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

#define PORT 7000
#define MAXLINE 20
int NUMROUTERS;
char* ROOTNAME;

void createRouter(char* router_name, int router_num);
unsigned char* makeControlPacket(uint8_t data[NUMROUTERS][NUMROUTERS]);
void print_bits ( void* buf, size_t size_in_bytes );

#endif /* simulation_h */