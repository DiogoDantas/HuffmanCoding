#include "huffman.h"

//------------------------------------------------------------------------------------
//--------------------------------- HUFFMAN ------------------------------------------
//------------------------------------------------------------------------------------

Huffman::Huffman(std::string infile, std::string outfile):
	input_size(0),
	output_size(0),
	compression_rate(0)
{
	input = new BitStream(infile, 1);
	output = new BitStream(outfile, 0);
	symbolsCount = input->size();

	Node *array;
	createNodes(&array);

	PQueue *queue = new PQueue();
	for(int i = 0; i < 256; i++){
		queue->enqueue(array+i);
	}

	tree = new HuffmanTree(queue);

	code_table = new CompactTable[256];
	symbol_table = new ConsultationTable[256];

	tree->generateTables(code_table, symbol_table);
}

Huffman::~Huffman(){
	delete input;
	delete output;
	delete tree;
}

void Huffman::createNodes(Node** array){
	*array = new Node[256];

	for(int i = 0; i < 256; i++){
		(*array)[i].setCode((unsigned char)i);
	}

	Byte s;

	while(!input->eof()){
		input->readByte(&s);
		(*array)[(unsigned int)s].increasePriority();
	}

	(*array)[(unsigned int)s].decreasePriority();

	return;
}

void Huffman::compress(){
	// Conta o número entradas da árvore e escreve
	output->writeUInt(0x0000);
	unsigned int count;
	for(count = 0; count < 256; count++)
		if(!code_table[count].priority)
			break;
	output->writeUInt(count);

	// Escreve a árvore
	for(int i = 0; i < count; i++){
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
	}

}

void Huffman::decompress(){
	// Ler e construir árvore
	input->reset();
	unsigned int count;
	input->readUInt(&count);

	Node *array = new Node[256];

	for(int i = 0; i < count; i++){
		input->readUInt(&code_table[i].priority);
		input->readByte(&code_table[i].code);
		array->setPriority(code_table[i].priority);
		array->setCode(code_table[i].code);
		array->setLeftChild(nullptr);
		array->setRightChild(nullptr);
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

void HuffmanTree::print_tree(Node* node) const {
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