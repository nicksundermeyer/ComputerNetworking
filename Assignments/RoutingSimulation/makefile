all: Routers.c PacketSender.c
	make routers
	make sender

routers: Routers.c Routers.h
	gcc-8 -c Routers.c -fopenmp -g -o Routers.o
	gcc-8 Routers.o -L. -fopenmp -g -o routers
	gcc-8 Routers.o -L. -fopenmp -g -o routers.exe

sender: PacketSender.c PacketSender.h
	gcc-8 -c PacketSender.c -fopenmp -g -o PacketSender.o
	gcc-8 PacketSender.o -L. -fopenmp -g -o packetsender
	gcc-8 PacketSender.o -L. -fopenmp -g -o packetsender.exe