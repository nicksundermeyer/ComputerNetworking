all: Head.c Node.c duSocket.c duSocket.h
	make head
	make node

head: Head.c duSocket.c
	gcc-8 -c Head.c -fopenmp -g -o Head.o
	gcc-8 -c duSocket.c -fopenmp -g -o duSocket.o
	gcc-8 Head.o duSocket.o -L. -lduSocket -fopenmp -g -o head
	gcc-8 Head.o duSocket.o -L. -lduSocket -fopenmp -g -o head.exe

node: Node.c duSocket.c
	gcc-8 -c Node.c -fopenmp -g -o Node.o
	gcc-8 -c duSocket.c -fopenmp -g -o duSocket.o
	gcc-8 Node.o duSocket.o -L. -lduSocket -fopenmp -g -o node
	gcc-8 Node.o duSocket.o -L. -lduSocket -fopenmp -g -o node.exe