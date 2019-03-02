// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h> // gethostbyname - IPv4

unsigned char* makePacket(uint16_t seq, unsigned char* data);
uint16_t checkSum(unsigned char* data);
void print_bits ( void* buf, size_t size_in_bytes) ;