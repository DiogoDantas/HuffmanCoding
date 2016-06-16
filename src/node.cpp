#include "node.h"

Node::Node(){}

Node::~Node(){}

int Node::getPriority() const {
	return this->priority;
}

unsigned char Node::getCode() const {
	return this->code;
}

Node* Node::getRightChild() const {
	return this->right_child;
}

Node* Node::getLeftChild() const {
	return this->left_child;
}

void Node::setPriority(const int priority){
	this->priority = priority;
}

void Node::increasePriority(){
	this->priority++;
}

void Node::decreasePriority(){
	this->priority--;
}

void Node::setCode(const unsigned char code){
	this->code = code;
}

void Node::setRightChild(Node* right_child){
	this->right_child = right_child;
}

void Node::setLeftChild(Node* left_child){
	this->left_child = left_child;
}