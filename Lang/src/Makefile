CC=g++
CFLAGS=-c -Wall	

all: run

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main.o

frontend.o: frontend.cpp
	$(CC) $(CFLAGS) frontend.cpp -o frontend.o

backend.o: backend.cpp
	$(CC) $(CFLAGS) backend.cpp -o backend.o

binarytree.o: binarytree.cpp
	$(CC) $(CFLAGS) binarytree.cpp -o binarytree.o

graphwiz.o: Tree/graphwiz.cpp
	$(CC) $(CFLAGS) Tree/graphwiz.cpp -o graphwiz.o

run: main.o frontend.o backend.o binarytree.o graphwiz.o
	$(CC) main.o frontend.o backend.o binarytree.o graphwiz.o -o main

clean:
	rm *.o *.exe *.stackdump  *.bin