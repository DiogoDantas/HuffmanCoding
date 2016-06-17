CC = g++
FLAGS = -std=c++11 -O2 -g3 -Wall -Werror -pedantic

.PHONY: debug clean Huffman

Huffman: *.o
	$(CC) *.o $(FLAGS) -o Huffman
	./Huffman

debug: *.o
	$(CC) *.o $(FLAGS) -o Huffman
	gdb ./Huffman

main.o: main.cpp
	$(CC) main.cpp $(FLAGS) -c

node.o: node.cpp
	$(CC) node.cpp $(FLAGS) -c

pqueue.o: pqueue.cpp
	$(CC) pqueue.cpp $(FLAGS) -c

huffman.o: huffman.cpp
	$(CC) huffman.cpp $(FLAGS) -c

huffmantree.o: huffmantree.cpp
	$(CC) huffmantree.cpp $(FLAGS) -c

bitstream.o: bitstream.cpp
	$(CC) bitstream.cpp $(FLAGS) -c


clean: 
	rm -f Huffman *.o

