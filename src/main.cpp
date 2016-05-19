#include "huffman.h"
#include "loader.h"
#include <cmath>


int main(int argc, char const *argv[])
{
	Node* array;
	Loader loader("/home/diogodantas/Downloads/teste.pdf");
	loader.createNodes(&array);

	double total = 0, potencial = 0;

	for(int i = 0; i < 256; i++){
		total += array[i].getPriority();
	}

	for(int i = 0; i < 256; i++){
		potencial += -log2(array[i].getPriority()/total) * array[i].getPriority();
	}

	Huffman* h1 = new Huffman();

	for(int i = 0; i < 256; i++){
		h1->insert_queue(array+i);
	}

	compact* code_table = new compact[256];

	h1->build_tree();
	//h1->print_tree(h1->getRoot());
	//std::cout<<std::endl;
	h1->generateTable(code_table);
	std::cout<<std::endl;

	//std::cout << (double)h1->bits/1024/1024/8 << " MB" << std::endl;
	//std::cout << potencial/1024/1024/8 << " MB" << std::endl;


		h1->fileCompress("/home/diogodantas/Downloads/teste.pdf", "/home/diogodantas/Testes/output.dc", code_table);
		h1->fileDescompress("/home/diogodantas/Testes/output.dc", "/home/diogodantas/Testes/descomprimido.pdf");
		

	return 0;
}