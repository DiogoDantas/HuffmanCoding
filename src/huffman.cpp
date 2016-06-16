#include "huffman.h"

//------------------------------------------------------------------------------------
//--------------------------------- HUFFMAN ------------------------------------------
//------------------------------------------------------------------------------------

Huffman::Huffman(std::string infile, std::string outfile){

	/*if(!outfile.compare("")){
		if(!infile.substr(infile.length()-4).compare(".huf")){
			outfile = infile.substr(0, infile.rfind("."));
		}
		else{
			outfile = infile + ".huf";
		}
	
		//outfile = infile;

		//if(outfile.rfind(".") != std::string::npos)
		//	outfile = outfile.substr(0, outfile.rfind("."));
		
		//outfile += ".huf";
	}*/

	std::cout << infile << "\t" << outfile << std::endl;

	input = new BitStream(infile, 1);
	output = new BitStream(outfile, 0);
	symbolsCount = input->size();
}

Huffman::~Huffman(){
	delete input;
	delete output;
	if(tree)
		delete tree;
}

void Huffman::createNodes(Node** array){
	*array = new Node[256];

	for(int i = 0; i < 256; i++){
		(*array)[i].setCode((unsigned char)i);
	}

	Byte s;

	while(!input->readByte(&s)){
		(*array)[(unsigned int)s].increasePriority();
	}
}

void Huffman::compress(){
	// Generate Tree
	input_size = symbolsCount;
	Node *array;
	createNodes(&array);

	PQueue *queue = new PQueue();
	for(int i = 0; i < 256; i++){
		queue->enqueue(array+i);
		if(array[i].getPriority())
			maximum_compression_rate -= log2((double)(array[i].getPriority())/input_size)*(array[i].getPriority());
	}
	maximum_compression_rate /= 8.0;

	tree = new HuffmanTree(queue);

	code_table = new CompactTable[256];
	symbol_table = new ConsultationTable[256];

	tree->generateTables(code_table, symbol_table);

	// Conta o número entradas da árvore e escreve
	output->writeUInt(0x0000);
	unsigned int count;
	for(count = 0; count < 256; count++)
		if(!code_table[count].priority)
			break;
	output->writeUInt(count);

	// Escreve a árvore
	for(unsigned int i = 0; i < count; i++){
		output->writeUInt(code_table[i].priority);
		output->writeByte(code_table[i].code);
	}
	output->writeULong(symbolsCount);

	// Escreve o conteúdo da mensagem
	input->reset();
	unsigned int code;
	Byte auxCode;
	while(!input->readByte(&auxCode)){									
		//input->readByte(&auxCode);								// Lê um byte do arquivo de entrada
		code = (unsigned int)auxCode;
		output->writeBits(symbol_table[code].symbol, symbol_table[code].size); // Escreve a saída adequada
		output_size += symbol_table[code].size;
	}
	output_size /= 8;
	compression_rate = 100*((input_size-output_size)/(double)input_size);
	maximum_compression_rate = 100*((input_size-maximum_compression_rate)/(double)input_size);
}

void Huffman::decompress(){
	// Ler e construir árvore
	input->reset();
	unsigned int count;
	input->readUInt(&count);
	input->readUInt(&count);

	Node *array = new Node[256];
	unsigned int priority;
	Byte code;

	for(unsigned int i = 0; i < count; i++){
		input->readUInt(&priority);
		input->readByte(&code);
		array[(unsigned int)code].setPriority(priority);
		array[(unsigned int)code].setCode(code);
	}

	PQueue *queue = new PQueue();
	for(int i = 0; i < 256; i++){
		queue->enqueue(array+i);
	}

	tree = new HuffmanTree(queue);


	input->readULong(&symbolsCount);
	Node *aux;
	while(symbolsCount-- > 0){
		aux = tree->getRoot();
		while(aux->getLeftChild() && aux->getRightChild()){
			if(input->readBit())
				aux = aux->getRightChild();
			else
				aux = aux->getLeftChild();
		}
		output->writeByte(aux->getCode());	
	}
}

void Huffman::info(){
	std::cout << "Input Size:\t" << input_size << " Bytes" << std::endl;
	std::cout << "Output Size:\t" << output_size << " Bytes" << std::endl;
	std::cout << "Compression:\t" << compression_rate << "%" << std::endl;
	std::cout << "Best Possible:\t" << maximum_compression_rate << "%" << std::endl;
}

