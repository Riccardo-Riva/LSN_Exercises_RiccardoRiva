CC = g++
CFLAGS = -Wall -O3 --std=c++11
CPP = MolDyn_NVE.cpp
EXE = MolDyn_NVE.exe

Compila : $(EXE)
Esegui : $(EXE) config.0 input.dat
	./MolDyn_NVE.exe

MolDyn_NVE.exe : MolDyn_NVE.cpp MolDyn_NVE.h library.h library.o
	$(CC) MolDyn_NVE.cpp library.o -o MolDyn_NVE.exe

library.o : library.cpp library.h
	$(CC) -c library.cpp

Continue : 
	cp config.final config.0
	cp old.final old.0

reset : 
	cp config.fcc config.0

clean :
	rm $(EXE) *.o
