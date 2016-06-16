#include "huffmantree.h"

//------------------------------------------------------------------------------------
//----------------------------- HUFFMANTREE ------------------------------------------
//------------------------------------------------------------------------------------

HuffmanTree::HuffmanTree(PQueue *queue){
	build_tree(queue);
}

HuffmanTree::~HuffmanTree(){}

Node* HuffmanTree::getRoot() const {
	return this->root;
}

void HuffmanTree::build_tree(PQueue *queue){

	if (queue->size() == 1){
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

	build_tree(queue);
}

void HuffmanTree::print_tree(const Node* node) const {
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

void HuffmanTree::generateTables(CompactTable* code_table, ConsultationTable* symbol_table){

	Byte aux[16] = {0};
	searchLeaves(root, aux, 0U, code_table, symbol_table);
}

void HuffmanTree::searchLeaves(Node* node, Byte* symbol, const unsigned int size, CompactTable* code_table, ConsultationTable* symbol_table){ 

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
		int n = size;
		unsigned char mask = 1;
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
	
	searchLeaves(node->getLeftChild(), symbol, size+1, code_table, symbol_table);

	symbol[0] |= 1;

	searchLeaves(node->getRightChild(), symbol, size+1, code_table, symbol_table);

	aux = 1U;

	for(int i = 0; i < 15; i++){
		symbol[i] >>= 1;
		symbol[i] ^= symbol[i+1] & aux ? ((Byte)1<<7) : 0;
	}
	symbol[3] <<= 1;
}