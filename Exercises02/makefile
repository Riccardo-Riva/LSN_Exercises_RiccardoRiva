CC = g++
CFLAGS = -Wall -O3 --std=c++11
CPP = Exercise02.1.cpp Exercise02.2.cpp
EXE = Exercise02.1.exe Exercise02.2.exe
LIBH = random.h library.h
LIBO = random.o library.o

Compila : $(EXE) 
Esegui : $(EXE) 
	./Exercise02.1.exe 
	./Exercise02.2.exe

Exercise02.1.exe : Exercise02.1.o $(LIBO) functions.o integral.o
	$(CC) $(LIBO) functions.o integral.o Exercise02.1.o -o $@
Exercise02.1.o : Exercise02.1.cpp $(LIBH) functions.h basicFunction.h integral.h
	$(CC) -c Exercise02.1.cpp -o $@ $(CFLAGS)
Exercise02.2.exe : Exercise02.2.o $(LIBO) randomWalk.o 
	$(CC) $(LIBO) randomWalk.o  Exercise02.2.o -o $@
Exercise02.2.o : Exercise02.2.cpp $(LIBH) randomWalk.h linearVector.h
	$(CC) -c Exercise02.2.cpp -o $@ $(CFLAGS)

randomWalk.o: randomWalk.cpp randomWalk.cpp random.h linearVector.h
	$(CC) -c randomWalk.cpp -o $@
integral.o : integral.cpp integral.h basicFunction.h functions.h random.h 
	$(CC) -c integral.cpp -o $@
functions.o : functions.cpp functions.h basicFunction.h
	$(CC) -c functions.cpp -o $@
random.o : random.cpp random.h
	$(CC) -c random.cpp -o $@ $(CFLAGS)
library.o : library.cpp library.h
	$(CC) -c library.cpp -o $@

clean :
	rm $(EXE) *.o
