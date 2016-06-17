#include "huffman.h"

int main(int argc, char const *argv[])
{
	Huffman* h1 = new Huffman();
	h1->compress("../files/black.bmp");
	h1->info();
	h1->decompress("../files/black.bmp.huf");
	delete h1;

	return 0;
}