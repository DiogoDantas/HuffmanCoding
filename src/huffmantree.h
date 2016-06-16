#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <iostream>
#include "pqueue.h"

typedef unsigned char Byte;

typedef struct {
	unsigned int priority;
	unsigned char code;
} CompactTable;

typedef struct {
	Byte symbol[16];
	unsigned int size;
} ConsultationTable;

class HuffmanTree{
public:
	HuffmanTree(PQueue *queue);
	~HuffmanTree();
	Node* getRoot() const;
	void build_tree(PQueue *queue);
	void print_tree(const Node* node) const;
	void generateTables(CompactTable* code_table, ConsultationTable* symbol_table);
	void searchLeaves(Node* node, Byte* symbol, const unsigned int size, CompactTable* code_table, ConsultationTable* symbol_table); 
private:
	Node *root = nullptr;
};

#endif //HUFFMAN_TREE_H