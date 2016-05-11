#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "huffman.h"
#include "pqueue.h"

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
	void generateTable();
private:
	void searchLeaves(Node* node, unsigned long symbol, int size);
	PQueue* queue;
	Node* root;
};

#endif // HUFFMAN_H
