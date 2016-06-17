#include "huffman.h"
#include <cstring>

int main(int argc, char const *argv[])
{
	std::string name = argv[2];
	Huffman* h1 = new Huffman();
	if(!strcmp(argv[1], "compress")){
		h1->compress("../files/" + name);
		h1->info();
	}
	else if(!strcmp(argv[1], "decompress")){
		h1->decompress("../files/" + name);
	}
	
	delete h1;

	return 0;
}