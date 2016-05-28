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

	Byte aux[16] = {0};
	searchLeaves(root, aux, 0U);
}

void Huffman::searchLeaves(Node* node, Byte* symbol, const unsigned int size){ 

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
		symbol_table[pos].size = size;
		std::copy(symbol, symbol+16, symbol_table[pos].symbol);

		bits += node->getPriority()*size;
		symbols += node->getPriority();

		int n = size;
		unsigned char mask = 1;
		std::cout << (unsigned int)(unsigned char)node->getCode() << "\t";

		while(n-- > 0){
			std::cout << ((mask<<n%64) & symbol[n/64] ? 1 : 0);
		}

		for(int i = 0; i < 10-size; i++) std::cout << " ";

		std::cout << "\t" << node->getPriority() << "\t" << size;

		std::cout << std::endl;
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

	Byte aux = (Byte)1 << 7;

	for(int i = 15; i > 0; i--){
		symbol[i] <<= 1;
		symbol[i] ^= symbol[i-1] & aux ? 1 : 0;
	}
	symbol[0] <<= 1;
	
	
/* LONG ---------------------------------
	symbol[3] <<= 1;
	symbol[3] ^= symbol[2] & aux ? 1 : 0;
	symbol[2] <<= 1;
	symbol[2] ^= symbol[1] & aux ? 1 : 0;
	symbol[1] <<= 1;
	symbol[1] ^= symbol[0] & aux ? 1 : 0;
	symbol[0] <<= 1;
// LONG --------------------------------*/
	
	searchLeaves(node->getLeftChild(), symbol, size+1);

	symbol[0] |= 1;

	searchLeaves(node->getRightChild(), symbol, size+1);

	aux = 1U;

	for(int i = 0; i < 15; i++){
		symbol[i] >>= 1;
		symbol[i] ^= symbol[i+1] & aux ? ((Byte)1<<7) : 0;
	}
	symbol[3] <<= 1;

/* LONG -----------------------------------
	symbol[0] >>= 1;
	symbol[0] ^= symbol[1] & aux ? (1UL<<63) : 0;
	symbol[1] <<= 1;
	symbol[1] ^= symbol[2] & aux ? (1UL<<63) : 0;
	symbol[2] <<= 1;
	symbol[2] ^= symbol[3] & aux ? (1UL<<63) : 0;
	symbol[3] <<= 1;
// LONG ------------------------------------*/
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

	// Escreve a árvore
	unsigned int count;
	for(count = 0; count < 256; count++)
		if(!code_table[count].priority)
			break;

	output_file.write((char*)&count, sizeof(unsigned int));

	for(int i = 0; i < count; i++){
		output_file.write((char*)&code_table[i], 5);
	}

	output_file.write((char*)&symbols, sizeof(unsigned long));

	// Escreve o conteúdo da mensagem
	unsigned int BLOCK_SIZE = 8;	// Usando long
	unsigned int code;
	char auxCode;
	Byte outputBlock = 0;
	Byte auxBlock = 0;
	unsigned int remainingBits;
	unsigned int freeBits = BLOCK_SIZE;
	Byte *symbol;

	while(!source_file.eof()){
		source_file.read(&auxCode, sizeof(char));				// Lê um byte do arquivo de entrada
		code = (unsigned int)(unsigned char)auxCode;

		remainingBits = symbol_table[code].size;				// Número de bits que ainda falta ser escrito na saída
		symbol = symbol_table[code].symbol;						// Símbolo a ser escrito na saída

		while(remainingBits > 0){								// Enquanto houverem bits para escrever...
			int piece = (remainingBits-1)/BLOCK_SIZE;			// Primeiro pedaço (cada um dos 4 longs do símbolo) que possui bits válidos
			int pieceSize = (remainingBits-1)%BLOCK_SIZE+1;		// Tamanho do primeiro Long válido

			auxBlock = symbol[piece];							// Copie este pedaço para auxBlock e posicione os bits
			auxBlock <<= BLOCK_SIZE-pieceSize;					// no local correto para escrever no bloco de saída
			auxBlock >>= BLOCK_SIZE-freeBits;						
			outputBlock |= auxBlock;

			if(pieceSize < freeBits){							// Ajusta o número de bits livres e o número de  				
				remainingBits -= pieceSize;						// bits que ainda falta ser lido
				freeBits -= pieceSize;
			}
			else{
				remainingBits -= freeBits;
				freeBits = BLOCK_SIZE;
				output_file.write((char*)&outputBlock, sizeof(Byte));
				outputBlock = 0;
			}
		}
	}

	if(freeBits > 0){
		outputBlock |= ((Byte)0 - (Byte)1) >> BLOCK_SIZE-freeBits;
		output_file.write((char*)&outputBlock, sizeof(Byte));
	}
	
	output_file.close();
	source_file.close();

}

void Huffman::fileDescompress(const std::string sourceFile, const std::string outputFile){

	std::ifstream source_file;
	std::ofstream output_file;

	source_file.open(sourceFile, std::fstream::binary | std::fstream::in);
	output_file.open(outputFile, std::fstream::binary | std::fstream::out);

	if((!source_file.good()) || (!output_file.good())){
		perror("open");
		exit(1);
	}

	// Ler e construir árvore
	unsigned int count;
	source_file.read((char*)&count, sizeof(unsigned int));

	Node* tmp;

	for(int i = 0; i < count; i++){
		source_file.read((char*)&code_table[i], 5);
		tmp = new Node;
		tmp->setPriority(code_table[i].priority);
		tmp->setCode(code_table[i].code);
		tmp->setLeftChild(nullptr);
		tmp->setRightChild(nullptr);
	}

	build_tree();

	source_file.read((char*)&symbols, sizeof(unsigned long));

	Node *aux;
	Byte mask = 0;
	Byte block;

	while(symbols-- > 0){
		aux = root;
		while(aux->getLeftChild() && aux->getRightChild()){

			if(!mask){
				source_file.read((char*)&block, sizeof(Byte));
				mask = (Byte)1 << 7;
			}

			if(mask & block)
				aux = aux->getRightChild();
			else
				aux = aux->getLeftChild();

			mask >>= 1;
		}
		
		unsigned char wrt = aux->getCode();
		output_file.write((char*)&wrt, sizeof(unsigned char));	
	}

	output_file.close();
	source_file.close();
}