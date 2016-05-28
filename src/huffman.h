#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <fstream>
#include "pqueue.h"

typedef unsigned char Byte;

typedef struct 
{
	unsigned int priority;
	unsigned char code;
} CompactTable;

typedef struct 
{
	Byte symbol[16];
	unsigned int size;
} ConsultationTable;

class Huffman
{
public:
	Huffman();
	~Huffman();
	void insert_queue(Node* node);
	void build_tree();
	void print_tree(Node* node) const;
	Node* getRoot() const;
	void generateTS();
	void generateTables();
	void fileCompress(const std::string sourceFile, const std::string outputFile);
	void fileDescompress(const std::string sourceFile, const std::string outputFile);

	unsigned long bits = 0;
	unsigned long symbols = 0;
	// MUDAR!!
	CompactTable* code_table;
	ConsultationTable* symbol_table;
private:
	void searchLeaves(Node* node, Byte* symbol, const unsigned int size);
	
	PQueue* queue;
	Node* root;
};

#endif // HUFFMAN_H
