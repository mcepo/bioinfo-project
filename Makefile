CC=g++
CFLAGS=-I

all: main generator

generator: generator.o
	$(CC)  -o generator generator.o

main: main.o edlib/edlib.o
	$(CC)  -o main main.o edlib/edlib.o
