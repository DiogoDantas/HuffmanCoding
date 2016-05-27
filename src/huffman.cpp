#include "huffman.h"
#include <algorithm>
#include <iostream>

Huffman::Huffman():root(nullptr){

	this->queue = new PQueue();
}

Huffman::~Huffman(){}

void Huffman::insert_queue(Node* node){
	if(node->getPriority() == 0) return;
	queue->enqueue(node);

}

Node* Huffman::getRoot() const {

	return this->root;
}

void Huffman::build_tree(){

	if (queue->size() == 1)
	{
		root = queue->top();
		return;
	}


	Node *left_node, *right_node, *new_node;

	new_node = new Node();

	left_node = queue->dequeue();
	right_node = queue->dequeue();

	new_node->setPriority(left_node->getPriority() + right_node->getPriority());

	new_node->setLeftChild(left_node);
	new_node->setRightChild(right_node);

	queue->enqueue(new_node);

	Huffman::build_tree();
}

void Huffman::print_tree(Node* node) const {
	if(node != NULL){
		std::cout << node->getPriority();

		if(node->getLeftChild() != NULL || node->getRightChild() != NULL){
			std::cout << "[";
			print_tree(node->getLeftChild());
			std::cout << " ";
			print_tree(node->getRightChild());
			std::cout << "]";
		}
	}

	return;
}

void Huffman::generateTables(){

	unsigned long aux[4] = {0UL, 0UL, 0UL, 0UL};
	searchLeaves(root, aux, 0U);
}

void Huffman::searchLeaves(Node* node, unsigned long* symbol, const unsigned int size){ 

	/**
		A função busca recursivamente por folhas. Quando encontra 
		uma folha, usa o símbolo gerado para criar uma entrada na
		tabela de símbolos.

	*/

	static int count = 0;
	if(node == NULL) return;
	if(node->getLeftChild() == NULL && node->getRightChild() == NULL){
		code_table[count].code = node->getCode();
		code_table[count++].priority = node->getPriority();
		unsigned int pos = (unsigned int)node->getCode();
		//symbol_table[pos].code = node->getCode();
		symbol_table[pos].size = size;
		std::copy(symbol, symbol+4, symbol_table[pos].symbol);

		bits += node->getPriority()*size;

		//int n = size;
		//unsigned long mask = 1;

		//while(n-- > 0){
		//	std::cout << ( (mask<<n%64) & symbol[n/64] ? 1 : 0);
		//}

		//std::cout << std::endl;
		return;
	}

	/**
		Esta parte final da função altera o símbolo a ser gerado 
		de maneira adequada e chama recursivamente a função sobre 
		os nós filhos. Na prática, a função move o código todo 1 bit 
		para a esquerda, gerando um bit 0 extra à direita. Então, 
		chama o filho da esquerda, passando o novo código. Ao retornar 
		deste filho, altera o último bit para que seja 1 e então 
		repassa para o filho da direita. Quando este último retorna, 
		o bit extra é finalmente apagado através de uma movimentação 
		de um bit para a direita, o que faz a função devolver o mesmo 
		símbolo que recebeu.
	*/

	unsigned long aux = 1UL << 63;
	
	symbol[3] <<= 1;
	symbol[3] ^= symbol[2] & aux ? 1 : 0;
	symbol[2] <<= 1;
	symbol[2] ^= symbol[1] & aux ? 1 : 0;
	symbol[1] <<= 1;
	symbol[1] ^= symbol[0] & aux ? 1 : 0;
	symbol[0] <<= 1;
	
	searchLeaves(node->getLeftChild(), symbol, size+1);

	symbol[0] |= 1;

	searchLeaves(node->getRightChild(), symbol, size+1);

	aux = 1;

	symbol[0] >>= 1;
	symbol[0] ^= symbol[1] & aux ? (1UL<<63) : 0;
	symbol[1] <<= 1;
	symbol[1] ^= symbol[2] & aux ? (1UL<<63) : 0;
	symbol[2] <<= 1;
	symbol[2] ^= symbol[3] & aux ? (1UL<<63) : 0;
	symbol[3] <<= 1;
}

void Huffman::fileCompress(const std::string sourceFile, const std::string outputFile){
	std::ifstream source_file;
	std::ofstream output_file;

	source_file.open(sourceFile, std::fstream::binary | std::fstream::in);
	output_file.open(outputFile, std::fstream::binary | std::fstream::out);

	if((!source_file.good()) || (!output_file.good())){
		perror("open");
		exit(1);
	}

	unsigned int BLOCK_SIZE = 64;	// Usando long
	unsigned char code;
	unsigned long outputBlock = 0UL;
	unsigned long auxBlock = 0L;
	unsigned int remainingBits;
	unsigned int freeBits = BLOCK_SIZE;
	unsigned long *symbol;

	while(!source_file.eof()){
		source_file >> code;									// Lê um byte do arquivo de entrada

		remainingBits = symbol_table[(unsigned int)code].size;	// Número de bits que ainda falta ser escrito na saída
		symbol = symbol_table[(unsigned int)code].symbol;		// Símbolo a ser escrito na saída

		while(remainingBits > 0){								// Enquanto houverem bits para escrever...
			int piece = (remainingBits-1)/BLOCK_SIZE;			// Primeiro pedaço (cada um dos 4 longs do símbolo) que possui bits válidos
			int pieceSize = (remainingBits-1)%BLOCK_SIZE+1;		// Tamanho do primeiro Long válido

			auxBlock = symbol[piece];							// Copie este pedaço para auxBlock e posicione os bits
			auxBlock <<= BLOCK_SIZE-pieceSize;					// no local correto para escrever no bloco de saída
			auxBlock >>= BLOCK_SIZE-freeBits;						
			outputBlock |= auxBlock;

			if(pieceSize <= freeBits){							// Ajuste o número de bits livres e o número de  				
				remainingBits -= pieceSize;						// bits que ainda falta ser lido
				freeBits -= pieceSize;
			}
			else{
				remainingBits -= freeBits;
				freeBits = BLOCK_SIZE;
				output_file.write((char*)&outputBlock, sizeof(unsigned long));;
				outputBlock = 0UL;
			}

		}

	}

	output_file.close();
	source_file.close();

}

void Huffman::fileDescompress(const std::string sourceFile, const std::string outputFile){

	std::ifstream source_file;
	std::ofstream output_file;

	CompactTable* code_table = new CompactTable[256];

	unsigned char byte;
	unsigned char byte_aux = 1<<7; // serve pra fazer o & binário com os bytes lidos pra ler bit a bit

	int padding;
	int i;
	int size_table;
	int size_file;
	int cursor = 0;

	unsigned char code;
	int priority;

	source_file.open(sourceFile, std::fstream::binary | std::fstream::in);
	output_file.open(outputFile, std::fstream::binary | std::fstream::out);

	if((!source_file.good()) || (!output_file.good())){
		perror("open");
		exit(1);
	}

}