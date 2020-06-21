CC = g++
CFLAGS = -Wall -O3 --std=c++11

Esegui : Monte_Carlo_ISING_1D.exe
	./Monte_Carlo_ISING_1D.exe 

Monte_Carlo_ISING_1D.exe : Monte_Carlo_ISING_1D.o random.o
	$(CC) random.o Monte_Carlo_ISING_1D.o -o Monte_Carlo_ISING_1D.exe
Monte_Carlo_ISING_1D.o : Monte_Carlo_ISING_1D.cpp
	$(CC) -c Monte_Carlo_ISING_1D.cpp -o Monte_Carlo_ISING_1D.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm -f *.o Monte_Carlo_ISING_1D.exe
reset :
	rm -rf energy.out heat_capacity.out magnetization.out susceptibility.out