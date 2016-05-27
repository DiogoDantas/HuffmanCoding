#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <fstream>
#include "pqueue.h"

#ifndef TABLE_SIZE
#define TABLE_SIZE 256
#endif

typedef struct 
{
	unsigned int priority;
	unsigned char code;
} CompactTable;

typedef struct 
{
	unsigned long symbol[4];
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
	// MUDAR!!
	CompactTable* code_table;
	ConsultationTable* symbol_table;
private:
	void searchLeaves(Node* node, unsigned long* symbol, const unsigned int size);
	
	PQueue* queue;
	Node* root;
};

#endif // HUFFMAN_H
