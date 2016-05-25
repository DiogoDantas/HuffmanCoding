#include "huffman.h"
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

void Huffman::searchLeaves(Node* node, unsigned long* symbol, const int size, compact* code_table){ 

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

void Huffman::fileCompress(const std::string sourceFile, const std::string outputFile, compact* code_table){
	std::ifstream source_file;
	std::ofstream output_file;

	unsigned char byte, buffer, $auxiliar;
	int aux, padding;
	char* tmp;

	char code;
	int priority;

	source_file.open(sourceFile, std::fstream::binary | std::fstream::in);
	output_file.open(outputFile, std::fstream::binary | std::fstream::out);

	if((!source_file.good()) || (!output_file.good())){
		perror("open");
		exit(1);
	}

	//Escrita do tamanho do Header
	int size_file = 0; // tamanho do arquivo
	int size_table = 0;//tamanho da tabela
	
	output_file.put(0);// espaço onde o padding vai ser escrito
	output_file << size_file; // local onde o tamanho do arquivo vai ser escrito
	output_file << size_table; // local onde o tamanho da tabela vai ser escrito

	// Escrita do Header
	
	int i = 0;
	int size = TABLE_SIZE;
	
	for (i = 0; i < TABLE_SIZE; ++i)
	{
		if(code_table[i].code != 0) break; 
	}

	if (i == size){
		std::cout<<"Arquivo vazio"<<std::endl;
		exit(1);
	}else{

		size_table = size - i;

		for (; i < size; ++i){
			
			output_file << code_table[i].code;
			output_file << code_table[i].priority;
		}

	}


	buffer = '\0';
	aux = 7;

			// $auxiliar fica 00000001
	$auxiliar =  '\0';
	$auxiliar = ~$auxiliar;
	$auxiliar = $auxiliar << 1;
	$auxiliar = ~$auxiliar;



	//Escrita dos códigos comprimidos
	while(!source_file.eof())
	{

		byte = source_file.get();

		code = code_table[byte].code;
		priority = code_table[byte].priority;

		tmp = &code;

		//Escrita dos códigos comprimidos
		
		//output_file << code;
		

		for (i = 0; tmp[i] != '\0'; ++i, aux-- )
		{
			if(aux < 0)
			{
				output_file.put(buffer);
				size_file++;
				buffer = '\0';
				aux = 7;

			}


			if(tmp[i] == '1'){
					buffer = (($auxiliar << aux) | buffer); // adicionando o byte a sua posição do byte. Se code[i] for 1, é só decrementar o aux, pois já estará um lá.
				}
			}

		}

		if(aux == 7){
			padding = 0;
		}else{
			padding = aux + 1;
			while(aux >= 0){ //padding
				buffer = (($auxiliar << aux) | buffer);
				aux--;
			}

			output_file.put(buffer);
			size_file++;
		}

		std::cout<<size_file<<std::endl;

		output_file.seekp(0,std::ios_base::beg);
		output_file.put((unsigned char)padding);
		output_file << size_file;
		output_file << size_table;

		output_file.close();
		source_file.close();

}

void Huffman::fileDescompress(const std::string sourceFile, const std::string outputFile){

	std::ifstream source_file;
	std::ofstream output_file;

	compact* code_table = new compact[256];

	unsigned char byte;
	unsigned char byte_aux = 1<<7; // serve pra fazer o & binário com os bytes lidos pra ler bit a bit

	int padding;
	int i;
	int size_table;
	int size_file;
	int cursor = 0;

	char code;
	int priority;

	source_file.open(sourceFile, std::fstream::binary | std::fstream::in);
	output_file.open(outputFile, std::fstream::binary | std::fstream::out);

	if((!source_file.good()) || (!output_file.good())){
		perror("open");
		exit(1);
	}

}