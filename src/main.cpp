#include "huffman.h"
#include <cstring>

int main(int argc, char const *argv[])
{
	if(argc < 3){
		std::cout << "Número insuficiente de argumentos" << std::endl;
		exit(0); 
	}

	Huffman* h1 = new Huffman();
	if(!strcmp(argv[1], "-c")){
		h1->compress("../files/" + std::string{argv[2]});
		h1->info();
	}
	else if(!strcmp(argv[1], "-d")){
		h1->decompress("../files/" + std::string{argv[2]});
	}
	else{
		std::cout << "Comando inválido" << std::endl;
	}
	
	delete h1;

	return 0;
}