#include "huffman.h"

int main(int argc, char const *argv[])
{
	Huffman* h1 = new Huffman("another", "compact");
	h1->compress();
	delete h1;

	Huffman* h2 = new Huffman("compact", "another2");
	h2->compress();
	delete h2;

	return 0;
}