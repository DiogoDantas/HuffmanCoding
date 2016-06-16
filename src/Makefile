CC = g++
FLAGS = -std=c++11 -O2 -g3

.PHONY: debug clean Huffman

Huffman: main.o node.o pqueue.o huffman.o bitstream.o
	$(CC) main.cpp node.cpp pqueue.cpp huffman.cpp bitstream.cpp $(FLAGS) -o Huffman
	./Huffman ../files/another.txt
	#make clean

debug:
	$(CC) main.cpp node.cpp pqueue.cpp huffman.cpp bitstream.cpp $(FLAGS) -o Huffman
	gdb ./Huffman

main.o: main.cpp
	$(CC) main.cpp $(FLAGS) -c

node.o: node.cpp
	$(CC) node.cpp $(FLAGS) -c

pqueue.o: pqueue.cpp
	$(CC) pqueue.cpp $(FLAGS) -c

huffman.o: huffman.cpp
	$(CC) huffman.cpp $(FLAGS) -c

bitstream.o: bitstream.cpp
	$(CC) bitstream.cpp $(FLAGS) -c


clean: 
	rm -f Huffman *.o

