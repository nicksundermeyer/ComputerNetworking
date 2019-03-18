all: simulation.c router.c
	make routers
	make sender

simulation: simulation.c simulation.h
	gcc-8 -c simulation.c -fopenmp -g -o simulation.o
	gcc-8 simulation.o -L. -fopenmp -g -o simulation
	gcc-8 simulation.o -L. -fopenmp -g -o simulation.exe

router: router.c router.h
	gcc-8 -c router.c -fopenmp -g -o router.o
	gcc-8 router.o -L. -fopenmp -g -o router
	gcc-8 router.o -L. -fopenmp -g -o router.exe