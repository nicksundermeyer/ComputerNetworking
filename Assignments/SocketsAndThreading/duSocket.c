#include "duSocket.h"

// Like new ServerSocket in Java
int setupServerSocket(int portno)
{
  // Get a socket of the right type
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("ERROR opening socket");
    exit(1);
  }

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr)); 

  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

  // Basically the machine we are on...
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig bype first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);

  // Bind the socket to the given port
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR on binding\n");
    exit(1);
  }

  return sockfd;
}

// Like new Socket in Java
int callServer(char* host, int portno)
{
    return 0;
}

// Like ss.accept() in Java
int serverSocketAccept(int serverSocket)
{
    return 0;
}

// Write an int over the given socket
void writeInt(int x, int socket)
{
    
}

// Read an int from the given socket
int readInt(int socket)
{
    return 0;
}