#include "huffman.h"

int main(int argc, char const *argv[])
{
	Huffman* h1 = new Huffman("../files/test.bmp", "../files/test.bmp.huf");
	h1->compress();
	h1->info();
	delete h1;

	Huffman* h2 = new Huffman("../files/test.bmp.huf", "../files/test2.bmp");
	h2->decompress();
	delete h2;

	return 0;
}