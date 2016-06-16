#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "pqueue.h"
#include <algorithm>
#include "bitstream.h"
#include <iostream> //////
#include <cmath>

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
	Huffman(std::string infile, std::string outfile = "");
	~Huffman();

	void createNodes(Node** array);
	void compress();
	void decompress();
	void info();
private:
	BitStream *input = nullptr;
	BitStream *output = nullptr;
	HuffmanTree *tree = nullptr;
	long input_size = 0;
	long output_size = 0;
	double compression_rate = 0.0;
	double maximum_compression_rate = 0.0;
	CompactTable* code_table = nullptr;
	ConsultationTable* symbol_table = nullptr;
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
	Node *root = nullptr;
};

#endif // HUFFMAN_H
