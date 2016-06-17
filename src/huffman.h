#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <algorithm>
#include <cmath>
#include "bitstream.h"
#include "huffmantree.h"

class Huffman
{
public:
	Huffman();
	~Huffman();

	void createNodes(Node** array);
	void compress(const std::string infile);
	void decompress(const std::string infile);
	void info()const;
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

#endif // HUFFMAN_H
