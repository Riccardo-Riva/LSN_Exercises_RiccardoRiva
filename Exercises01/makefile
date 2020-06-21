CC = g++
CFLAGS = -Wall -O3 --std=c++11
CPP = Exercise01.1.cpp Exercise01.2.cpp Exercise01.3.cpp
EXE = Exercise01.1.exe Exercise01.2.exe Exercise01.3.exe
LIBH = random.h functions.h 
LIBO = random.o functions.o

Compila : $(EXE) 
Esegui : $(EXE) 
	./Exercise01.1.exe 
	./Exercise01.2.exe
	./Exercise01.3.exe

Exercise01.1.exe : Exercise01.1.o $(LIBO)
	$(CC) $(LIBO) Exercise01.1.o -o $@
Exercise01.1.o : Exercise01.1.cpp $(LIBH)
	$(CC) -c Exercise01.1.cpp -o $@ $(CFLAGS)
Exercise01.2.exe : Exercise01.2.o $(LIBO)
	$(CC) $(LIBO) Exercise01.2.o -o $@
Exercise01.2.o : Exercise01.2.cpp $(LIBH)
	$(CC) -c Exercise01.2.cpp -o $@ $(CFLAGS)
Exercise01.3.exe : Exercise01.3.o $(LIBO)
	$(CC) $(LIBO) Exercise01.3.o -o $@
Exercise01.3.o : Exercise01.3.cpp $(LIBH)
	$(CC) -c Exercise01.3.cpp -o $@ $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o $@ $(CFLAGS)
functions.o : functions.cpp functions.h
	$(CC) -c functions.cpp -o $@

clean :
	rm $(EXE) *.o
