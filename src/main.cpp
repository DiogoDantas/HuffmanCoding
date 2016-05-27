#include "huffman.h"
#include "loader.h"
#include <cmath>


int main(int argc, char const *argv[])
{
	Node* array;
	Loader loader("01 - crossing field.mp3");
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

	CompactTable* code_table = new CompactTable[256];
	ConsultationTable* consultation_table = new ConsultationTable[256];
	h1->code_table = code_table;
	h1->symbol_table = consultation_table;

	h1->build_tree();
	h1->generateTables();
	std::cout<<std::endl;

	h1->fileCompress("01 - crossing field.mp3", "saida.out");

	std::cout << (double)h1->bits/1024/1024/8 << " MB" << std::endl;
	std::cout << potencial/1024/1024/8 << " MB" << std::endl;


		//h1->fileCompress("/home/diogodantas/Downloads/teste.pdf", "/home/diogodantas/Testes/output.dc", code_table);
		//h1->fileDescompress("/home/diogodantas/Testes/output.dc", "/home/diogodantas/Testes/descomprimido.pdf");
		

	return 0;
}