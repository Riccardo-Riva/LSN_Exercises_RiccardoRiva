CC = g++
CFLAGS = -Wall -O3 --std=c++11
CPP = Exercise03.1.cpp
EXE = Exercise03.1.exe
LIBH = random.h library.h
LIBO = random.o library.o

Compila : $(EXE) 
Esegui : $(EXE) 
	./Exercise03.1.exe
Exercise03.1.exe : Exercise03.1.o $(LIBO) brownianMotion.o
	$(CC) $(LIBO) brownianMotion.o Exercise03.1.o -o $@
Exercise03.1.o : Exercise03.1.cpp $(LIBH) brownianMotion.h
	$(CC) -c Exercise03.1.cpp -o $@ $(CFLAGS)

brownianMotion.o : brownianMotion.cpp brownianMotion.h random.h
	$(CC) -c brownianMotion.cpp -o $@ 
random.o : random.cpp random.h
	$(CC) -c random.cpp -o $@ $(CFLAGS)
library.o : library.cpp library.h
	$(CC) -c library.cpp -o $@

clean :
	rm $(EXE) *.o
