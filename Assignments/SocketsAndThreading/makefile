all: Head.c Node.c duSocket.c duSocket.h
	make head
	make node

head: Head.c duSocket.c
	clang -c Head.c -g -o Head.o
	clang -c duSocket.c -g -o duSocket.o
	clang Head.o duSocket.o -g -o head
	clang Head.o duSocket.o -g -o head.exe

node: Node.c duSocket.c
	clang -c Node.c -g -o Node.o
	clang -c duSocket.c -g -o duSocket.o
	clang Node.o duSocket.o -g -o node
	clang Node.o duSocket.o -g -o node.exe