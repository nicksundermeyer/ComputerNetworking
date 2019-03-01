all: UDPclient.c UDPserver.c
	make client
	make server

client: UDPclient.c UDPclient.h
	gcc -c UDPclient.c -fopenmp -g -o UDPclient.o
	gcc UDPclient.o -L. -fopenmp -g -o client
	gcc UDPclient.o -L. -fopenmp -g -o client.exe

server: UDPserver.c UDPserver.h
	gcc -c UDPserver.c -fopenmp -g -o UDPserver.o
	gcc UDPserver.o -L. -fopenmp -g -o server
	gcc UDPserver.o -L. -fopenmp -g -o server.exe