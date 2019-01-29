#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int setupServerSocket(int portno); // Like new ServerSocket in Java
int callServer(char* host, int portno); // Like new Socket in Java
int serverSocketAccept(int serverSocket); // Like ss.accept() in Java
void writeInt(int x, int socket); // Write an int over the given socket
int readInt(int socket); // Read an int from the given socket