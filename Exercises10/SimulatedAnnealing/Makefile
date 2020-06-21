CC = g++
CFLAGS = -Wall -O3 --std=c++11
CPP = SA.cpp
EXE = SA.exe Cities.exe
LIBH = random.h individual.h metropolisSA.h
LIBO = random.o individual.o metropolisSA.o

Compila : $(EXE) 
Esegui : SA.exe 
	./SA.exe

SA.exe : SA.o $(LIBO)
	$(CC) $(LIBO) SA.o -o $@
SA.o : SA.cpp $(LIBH) linearVector.h
	$(CC) -c SA.cpp -o $@ $(CFLAGS)

Cities.exe : Cities.cpp random.o random.h
	$(CC) random.o Cities.cpp -o Cities.exe

random.o : random.cpp random.h
	$(CC) -c random.cpp -o $@ $(CFLAGS)
individual.o : individual.cpp individual.h
	$(CC) -c individual.cpp -o $@ $(CFLAGS)
metropolisSA.o : metropolisSA.cpp metropolisSA.h
	$(CC) -c metropolisSA.cpp -o $@ $(CFLAGS)

clean :
	rm -rf *.exe *.o