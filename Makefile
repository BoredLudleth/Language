CC=g++
CFLAGS=-c -Wall	

all: run_frontandback run_coder run_stack

frontandback.o: ./Lang/src/main.cpp
	$(CC) $(CFLAGS) ./Lang/src/main.cpp -o ./Lang/obj/main.o

frontend.o: ./Lang/src/frontend.cpp
	$(CC) $(CFLAGS) ./Lang/src/frontend.cpp -o ./Lang/obj/frontend.o

backend.o: ./Lang/src/backend.cpp
	$(CC) $(CFLAGS) ./Lang/src/backend.cpp -o ./Lang/obj/backend.o

binarytree.o: ./Lang/src/binarytree.cpp
	$(CC) $(CFLAGS) ./Lang/src/binarytree.cpp -o ./Lang/obj/binarytree.o

graphwiz.o: ./Lang/src/Tree/graphwiz.cpp
	$(CC) $(CFLAGS) ./Lang/src/Tree/graphwiz.cpp -o ./Lang/obj/graphwiz.o

run_frontandback: frontandback.o frontend.o backend.o binarytree.o graphwiz.o
	$(CC) ./Lang/obj/main.o ./Lang/obj/frontend.o ./Lang/obj/backend.o ./Lang/obj/binarytree.o ./Lang/obj/graphwiz.o -o frontandback

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
	rm *.o *.exe *.stackdump *.asm *.bin ./CPU/obj/*.o ./Lang/obj/*.o
