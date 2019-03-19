#ifndef demo_h
#define demo_h

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

#define NUMROUTERS 3

unsigned char* makePacket(uint16_t src, uint16_t dest, unsigned char* data);
unsigned char* makeControlPacket(uint8_t data[NUMROUTERS][NUMROUTERS]);
void print_bits ( void* buf, size_t size_in_bytes );

#endif