#include "huffman.h"
#include <iostream>

Huffman::Huffman(){

	this->queue = new PQueue();
	this->root = NULL;
}
Huffman::~Huffman(){}

void Huffman::insert_queue(Node* node){
	//queue->insert(node);
	if(node->getPriority() == 0) return;
	queue->enqueue(node);
}

Node* Huffman::getRoot(){

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

void Huffman::print_tree(Node* node)
{
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

void Huffman::generateTable(compact* code_table){

	unsigned long aux[4];
	aux[0] = 0UL;
	aux[1] = 0UL;
	aux[2] = 0UL;
	aux[3] = 0UL;
	searchLeaves(root, aux, 0, code_table);

	// procurar folhas
	// contar o caminho durante a busca
	// criar a entrada na tabela
}

void Huffman::searchLeaves(Node* node, unsigned long* symbol, int size, compact* code_table){ 

	static int count = 0;
	if(node == NULL) return;
	if(node->getLeftChild() == NULL && node->getRightChild() == NULL){
		//std::cout << (unsigned)(unsigned char)node->getCode() << '\t';
		code_table[count].code = node->getCode();
		code_table[count++].priority = node->getPriority();


		bits += node->getPriority()*size;

		int n = size;
		unsigned long filter = 1;


		while(n-- > 0){
			std::cout << ( (filter<<n%64) & symbol[n/64] ? 1 : 0);
		}

		std::cout << std::endl;
		return;
	}

	unsigned long aux = 1UL << 63;
	
	symbol[3] <<= 1;
	symbol[3] ^= symbol[2] & aux ? 1 : 0;
	symbol[2] <<= 1;
	symbol[2] ^= symbol[1] & aux ? 1 : 0;
	symbol[1] <<= 1;
	symbol[1] ^= symbol[0] & aux ? 1 : 0;
	symbol[0] <<= 1;
	
	searchLeaves(node->getLeftChild(), symbol, size+1, code_table);

	symbol[0] |= 1;

	searchLeaves(node->getRightChild(), symbol, size+1, code_table);

	aux = 1;

	symbol[0] >>= 1;
	symbol[0] ^= symbol[1] & aux ? (1UL<<63) : 0;
	symbol[1] <<= 1;
	symbol[1] ^= symbol[2] & aux ? (1UL<<63) : 0;
	symbol[2] <<= 1;
	symbol[2] ^= symbol[3] & aux ? (1UL<<63) : 0;
	symbol[3] <<= 1;
}

void Huffman::fileCompress(std::string sourceFile, std::string outputFile, compact* code_table){
	std::ifstream source_file;
	std::ofstream output_file;

	source_file.open(sourceFile, std::fstream::binary | std::fstream::in);
	output_file.open(outputFile, std::fstream::binary | std::fstream::out);

	if((!source_file.good()) || (!output_file.good())){
		perror("open");
		exit(1);
	}
}