#include "huffman.h"

Huffman::Huffman(){

	this->queue = new PQueue();
	this->root = NULL;
}
Huffman::~Huffman(){}

void Huffman::insert_queue(Node* node){
	queue->insert(node);
}

Node* Huffman::getRoot(){

	return this->root;
}

void Huffman::build_tree(){

	Node *left_node, *right_node, *new_node;

	new_node = new Node();

	left_node = queue->dequeue();
	right_node = queue->dequeue();

	new_node->setPriority(left_node->getPriority() + right_node->getPriority());

	new_node->setLeftChild(left_node);
	new_node->setRightChild(right_node);

	queue->enqueue(new_node);

	if (queue->size() == 1)
	{
		root = queue->top();
		return;
	}

	Huffman::build_tree();
}

void Huffman::print_tree(Node* node)
{
	if(node != NULL){
		std::cout<<node->getPriority();
		if(node->getLeftChild() != NULL)
			std::cout<<"(";
		print_tree(node->getLeftChild());
		print_tree(node->getRightChild());
		if(node->getRightChild() != NULL)
			std::cout<<")";
	}

	return;
}