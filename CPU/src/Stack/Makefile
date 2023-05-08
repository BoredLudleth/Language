CC=gcc
CFLAGS=-c -Wall	

all: run_stack

main.o: ./src/main.cpp
	$(CC) $(CFLAGS) ./src/main.cpp -o ./obj/main.o

stack.o: ./src/stack.cpp
	$(CC) $(CFLAGS) ./src/stack.cpp -o ./obj/stack.o

run_stack: main.o stack.o
	$(CC) ./obj/main.o ./obj/stack.o -o run_stack

clean:
	rm run_stack
	rm ./obj/*.o
