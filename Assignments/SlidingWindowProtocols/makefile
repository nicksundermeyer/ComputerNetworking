all: UDPclient.cpp UDPserver.cpp
	make client
	make server

client: UDPclient.cpp
	gcc-8 -c UDPclient.cpp -fopenmp -g -o UDPclient.o
	gcc-8 UDPclient.o -L. -fopenmp -g -o client

server: UDPserver.cpp
	gcc-8 -c UDPserver.cpp -fopenmp -g -o UDPserver.o
	gcc-8 UDPserver.o -L. -fopenmp -g -o server
