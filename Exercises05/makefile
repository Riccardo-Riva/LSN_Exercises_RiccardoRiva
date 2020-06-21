CC = g++
CFLAGS = -Wall -O3 --std=c++11
CPP = Exercise05.1.cpp
EXE = Exercise05.1.exe
LIBH = random.h library.h linearVector.h probabilityDensity.h pdf.h
LIBC = random.cpp library.cpp metropolis.cpp 

Compila : $(EXE)
Esegui : $(EXE) 
	./Exercise05.1.exe

Exercise05.1.exe : $(LIBH) $(LIBC) Exercise05.1.cpp
	$(CC) Exercise05.1.cpp random.cpp library.cpp metropolis.cpp -o Exercise05.1.exe

clean :
	rm $(EXE) *.o