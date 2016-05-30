#include "huffman.h"
#include "loader.h"
#include <cmath>


int main(int argc, char const *argv[])
{

	long total = 0, potencial = 0;
	long input_size = 0, output_size, compression_rate, table_size, entry = 0; 

	Node* array;
	Loader loader("01 - crossing field.mp3");
	loader.createNodes(&array);

	for(int i = 0; i < 256; i++){
		total += array[i].getPriority();
		if(array[i].getPriority())
			entry++;
	}

	input_size = total;

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

	h1->fileCompress("01 - crossing field.mp3", "compressed");
	h1->fileDescompress("compressed", "01 - crossing field2.mp3");

	table_size = 5 + (5*entry);
	output_size = h1->bits/8 + table_size;

	compression_rate = (1 - (double)output_size/input_size)*100;

	std::cout << "Tamanho da Entrada: \t" << input_size << " Bytes" << std::endl;
	std::cout << "Tamanho da Saída: \t" <<  output_size << " Bytes" << std::endl;
	std::cout << "Taxa de compressão: \t" << compression_rate << "%" << std::endl;
	//std::cout << "Máximo possível: " << potencial/8 << " Bytes" << std::endl;		

	return 0;
}