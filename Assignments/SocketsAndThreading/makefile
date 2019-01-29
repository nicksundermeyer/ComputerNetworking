all: Head.c Node.c duSocket.c duSocket.h
	make head
	make node

head: Head.c duSocket.c
	gcc -c Head.c -o Head.o
	gcc -c duSocket.c -o duSocket.o
	gcc Head.o duSocket.o -o head
	gcc Head.o duSocket.o -o head.exe

node: Node.c duSocket.c
	gcc -c Node.c -o Node.o
	gcc -c duSocket.c -o duSocket.o
	gcc Node.o duSocket.o -o node
	gcc Node.o duSocket.o -o node.exe