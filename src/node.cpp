#include "node.h"

Node::Node(){
	/*
	* Inicialização dos atributos dos atributos da classe
	*/
	this->code = 0;
	this->priority = 0;
	this->right_child = NULL;
	this->left_child = NULL;

}

Node::~Node(){;} // Instrução Vazia

int Node::getPriority(){
	return this->priority;
}

char Node::getCode(){
	return this->code;
}

Node* Node::getRightChild(){
	return this->right_child;
}

Node* Node::getLeftChild(){
	return this->left_child;
}

void Node::setPriority(int priority){
	this->priority = priority;
}

void Node::increasePriority(){
	this->priority++;
}

void Node::setCode(char code){
	this->code = code;
}

void Node::setRightChild(Node* right_child){
	this->right_child = right_child;
}

void Node::setLeftChild(Node* left_child){
	this->left_child = left_child;
}