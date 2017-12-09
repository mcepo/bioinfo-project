CC=g++ 
CFLAGS=-I -Wall -no-pie -pg

all: main generator

generator: generator.o
	$(CC)  -o generator generator.o

main: main.o edlib/edlib.o FourRussians.o
	$(CC)  $(CFLAGS) -o main main.o edlib/edlib.o FourRussians.o
	
clean:
	rm *.o main generator
