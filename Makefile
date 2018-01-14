
## dodao sam opcije -no-pie i -pg kako bi aktivirao profilera prilikom
## kompajliranja
## nakon pokretanja programa generirati će se datoteka gmon.out
## nakon čega je potrebno pokrenuti naredbu
## gprof main gmon.out >> <naziv_datoteke_za_rezultat>
## -no-pie
## CC=g++ -no-pie -pg
CC=g++
LFLAGS= -Wall -std=c++11
CFLAGS= -c -g -O2 -std=c++11

all: main generator

generator: generator.o
	$(CC) generator.o -o generator

main: main.o edlib.o FourRussians.o
	$(CC)  $(LFLAGS) main.o edlib.o FourRussians.o -o main
	
generator.o: generator.cpp
	$(CC) $(CFLAGS) generator.cpp
	
main.o : main.cpp FourRussians.o edlib.o
	$(CC) $(CFLAGS) main.cpp
	
FourRussians.o : FourRussians.cpp FourRussians.h
	$(CC) $(CFLAGS) FourRussians.cpp
	
edlib.o : edlib/edlib.cpp edlib/edlib.h
	$(CC) $(CFLAGS) edlib/edlib.cpp
		
clean:
	rm *.o main generator
	
	