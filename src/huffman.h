#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <fstream>
#include "pqueue.h"

#ifndef TABLE_SIZE
#define TABLE_SIZE 256
#endif

typedef struct 
{
		int priority;
		char code;
}compact;

class Huffman
{
public:
	Huffman();
	~Huffman();
	void insert_queue(Node* node);
	void build_tree();
	void print_tree(Node* node);
	Node* getRoot();
	void generateTS();
	void generateTable(compact* code_table);
	void fileCompress(std::string sourceFile, std::string outputFile, compact* code_table);

	unsigned long bits = 0;

private:
	void searchLeaves(Node* node, unsigned long* symbol, int size, compact* code_table);
	PQueue* queue;
	Node* root;
};

#endif // HUFFMAN_H
