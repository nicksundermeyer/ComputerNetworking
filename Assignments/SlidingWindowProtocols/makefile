all: UDPclient.c UDPserver.c
	make client
	make server

client: UDPclient.c UDPclient.h
	gcc -c UDPclient.c -g -o UDPclient.o
	gcc UDPclient.o -L. -g -o client
	gcc UDPclient.o -L. -g -o client.exe

server: UDPserver.c UDPserver.h
	gcc -c UDPserver.c -g -o UDPserver.o
	gcc UDPserver.o -L. -g -o server
	gcc UDPserver.o -L. -g -o server.exe