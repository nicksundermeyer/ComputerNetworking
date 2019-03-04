all: UDPclient.c UDPserver.c
	make client
	make server

client: UDPclient.c UDPclient.h
	gcc-8 -c UDPclient.c -fopenmp -g -o UDPclient.o
	gcc-8 UDPclient.o -L. -fopenmp -g -o client
	gcc-8 UDPclient.o -L. -fopenmp -g -o client.exe

server: UDPserver.c UDPserver.h
	gcc-8 -c UDPserver.c -fopenmp -g -o UDPserver.o
	gcc-8 UDPserver.o -L. -fopenmp -g -o server
	gcc-8 UDPserver.o -L. -fopenmp -g -o server.exe