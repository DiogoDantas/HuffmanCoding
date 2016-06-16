#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "pqueue.h"
#include <algorithm>
#include "bitstream.h"
#include <iostream> //////

typedef unsigned char Byte;

typedef struct {
	unsigned int priority;
	unsigned char code;
} CompactTable;

typedef struct {
	Byte symbol[16];
	unsigned int size;
} ConsultationTable;

class HuffmanTree;

class Huffman
{
public:
	Huffman(std::string infile, std::string outfile);
	~Huffman();

	void createNodes(Node** array);
	void compress();
	void decompress();
private:
	BitStream *input;
	BitStream *output;
	HuffmanTree *tree;
	long input_size;
	long output_size;
	long compression_rate;
	CompactTable* code_table;
	ConsultationTable* symbol_table;
	unsigned long symbolsCount = 0;
};

class HuffmanTree{
public:
	HuffmanTree(PQueue *queue);
	~HuffmanTree();
	Node* getRoot() const;
	void build_tree(PQueue *queue);
	void print_tree(Node* node) const;
	void generateTables(CompactTable* code_table, ConsultationTable* symbol_table);
	void searchLeaves(Node* node, Byte* symbol, const unsigned int size, CompactTable* code_table, ConsultationTable* symbol_table); 
private:
	Node *root;
};

#endif // HUFFMAN_H
