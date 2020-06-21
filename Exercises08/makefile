CC = g++
CFLAGS = -Wall -O3 --std=c++11
CPP = variational.cpp ground_state.cpp
EXE = variational.exe ground_state.exe
LIBH = random.h library.h linearVector.h probabilityDensity.h pdf.h
LIBC = random.cpp library.cpp metropolis.cpp 

Compila : $(EXE)
Esegui : $(EXE) 
	./ground_state.exe

variational.exe : $(LIBH) $(LIBC) variational.cpp
	$(CC) variational.cpp random.cpp library.cpp metropolis.cpp -o variational.exe

ground_state.exe : $(LIBH) $(LIBC) ground_state.cpp
	$(CC) ground_state.cpp random.cpp library.cpp metropolis.cpp -o ground_state.exe

clean :
	rm -rf $(EXE) *.o
