CC = g++
MPI = mpicxx
CFLAGS = -Wall -O3 --std=c++11
CPP = TSP.cpp
EXE = TSP.exe FirstGen.exe
LIBH = random.h individual.h
LIBO = random.o individual.o

Compila : $(EXE) 
Esegui : TSP.exe 
	mpiexec -np 4 ./TSP.exe

TSP.exe : TSP.o $(LIBO) population.o
	$(MPI) $(LIBO) population.o TSP.o -o $@
TSP.o : TSP.cpp $(LIBH) population.h linearVector.h
	$(MPI) -c TSP.cpp -o $@ $(CFLAGS)

FirstGen.exe : FirstGen.o $(LIBO)
	$(MPI) $(LIBO) FirstGen.o -o $@
FirstGen.o : FirstGen.cpp $(LIBH)
	$(MPI) -c FirstGen.cpp -o $@ $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o $@ $(CFLAGS)
individual.o : individual.cpp individual.h
	$(CC) -c individual.cpp -o $@ $(CFLAGS)
population.o : population.cpp population.h individual.h
	$(CC) -c population.cpp -o $@ $(CFLAGS)

clean :
	rm -rf *.exe *.o
