#include "huffman.h"
#include "loader.h"


int main(int argc, char const *argv[])
{
	
	//Node* n1 = new Node(); Node* n2 = new Node(); Node* n3 = new Node(); Node* n4 = new Node(); Node* n5 = new Node();
	//n1->setPriority(10); n2->setPriority(50); n3->setPriority(30); n4->setPriority(5); n5->setPriority(1);

	Node* array;
	Loader loader("nada");
	loader.createNodes(&array);

	Huffman* h1 = new Huffman();

	for(int i = 0; i < 256; i++){
		h1->insert_queue(array+i);
	}

	//h1->insert_queue(n1); h1->insert_queue(n2); h1->insert_queue(n3); h1->insert_queue(n4); h1->insert_queue(n5);

	h1->build_tree();
	h1->print_tree(h1->getRoot());
	std::cout<<std::endl;
	
	return 0;
}