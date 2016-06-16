#include "huffman.h"

int main(int argc, char const *argv[])
{
	Huffman* h1 = new Huffman("test.bmp", "compact");
	h1->compress();
	delete h1;

	Huffman* h2 = new Huffman("compact", "another2.bmp");
	h2->decompress();
	delete h2;

	return 0;
}