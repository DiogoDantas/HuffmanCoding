#include "huffman.h"

int main(int argc, char const *argv[])
{
	Huffman* h1 = new Huffman();
	h1->compress("../files/01 - crossing field.mp3");
	h1->info();
	h1->decompress("../files/01 - crossing field.mp3.huf");
	delete h1;

	return 0;
}