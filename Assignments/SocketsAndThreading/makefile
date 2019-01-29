all: Head.c Node.c duSocket.c duSocket.h
	clang Head.c -o Head
	clang Node.c -o Node
	clang duSocket.c -c

head: Head.c duSocket.c duSocket.h
	clang Head.c -o Head

node: Node.c duSocket.c duSocket.h
	clang Node.c -o Node