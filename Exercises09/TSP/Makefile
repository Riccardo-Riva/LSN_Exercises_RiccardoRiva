CC = g++
CFLAGS = -Wall -O3 --std=c++11
CPP = TSP.cpp
EXE = TSP.exe FirstGen.exe Cities.exe
LIBH = random.h individual.h
LIBO = random.o individual.o

Compila : $(EXE) 
Esegui : TSP.exe 
	./TSP.exe

TSP.exe : TSP.o $(LIBO) population.o
	$(CC) $(LIBO) population.o TSP.o -o $@
TSP.o : TSP.cpp $(LIBH) population.h linearVector.h
	$(CC) -c TSP.cpp -o $@ $(CFLAGS)

FirstGen.exe : FirstGen.o $(LIBO)
	$(CC) $(LIBO) FirstGen.o -o $@
FirstGen.o : FirstGen.cpp $(LIBH)
	$(CC) -c FirstGen.cpp -o $@ $(CFLAGS)

Cities.exe : Cities.cpp random.o random.h
	$(CC) random.o Cities.cpp -o Cities.exe

random.o : random.cpp random.h
	$(CC) -c random.cpp -o $@ $(CFLAGS)
individual.o : individual.cpp individual.h
	$(CC) -c individual.cpp -o $@ $(CFLAGS)
population.o : population.cpp population.h individual.h
	$(CC) -c population.cpp -o $@ $(CFLAGS)

clean :
	rm -rf *.exe *.o
