#include "huffman.h"

int main(int argc, char const *argv[])
{
	Huffman* h1 = new Huffman("../files/another.txt", "../files/compact.huf");
	//Huffman* h1 = new Huffman(argv[1]);
	h1->compress();
	h1->info();
	delete h1;

	Huffman* h2 = new Huffman("../files/compact.huf", "../files/another2.txt");
	h2->decompress();
	delete h2;

	return 0;
}