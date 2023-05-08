CC=g++
CFLAGS=-c -Wall	

all: run_coder run_stack

main.o:
	$(CC) $(CFLAGS) ./CPU/src/main.cpp -o ./CPU/obj/main.o

stack.o:
	$(CC) $(CFLAGS) ./CPU/src/Stack/src/stack.cpp -o ./CPU/obj/stack.o

stackoperations.o:
	$(CC) $(CFLAGS) ./CPU/src/Stack/src/stackoperations.cpp -o ./CPU/obj/stackoperations.o

coder.o:
	$(CC) $(CFLAGS) ./CPU/src/asm/coder.cpp -o ./CPU/obj/coder.o

coderfunctions.o:
	$(CC) $(CFLAGS) ./CPU/src/asm/coderfunctions.cpp -o ./CPU/obj/coderfunctions.o

run_coder: coder.o coderfunctions.o
	$(CC) ./CPU/obj/coder.o ./CPU/obj/coderfunctions.o -o run_coder

run_stack: main.o stack.o stackoperations.o
	$(CC) ./CPU/obj/main.o ./CPU/obj/stack.o ./CPU/obj/stackoperations.o -o run_stack

clean:
	rm *.o *.exe *.stackdump *.code *.bin ./CPU/obj/*.obj
