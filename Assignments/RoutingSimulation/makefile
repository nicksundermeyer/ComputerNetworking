all: simulation.c demo.c
	make demo
	make sender

simulation: simulation.c simulation.h
	gcc-8 -c simulation.c -fopenmp -g -o simulation.o
	gcc-8 simulation.o -L. -fopenmp -g -o simulation
	gcc-8 simulation.o -L. -fopenmp -g -o simulation.exe

demo: demo.c demo.h
	gcc-8 -c demo.c -fopenmp -g -o demo.o
	gcc-8 demo.o -L. -fopenmp -g -o demo
	gcc-8 demo.o -L. -fopenmp -g -o demo.exe