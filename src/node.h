#ifndef NODE_H
#define NODE_H


#include <iostream>
#include <cstdlib>

class Node
{
public:
	Node();
	~Node();
	Node* getLeftChild() const;
	Node* getRightChild() const;
	int getPriority() const;
	unsigned char getCode() const;
	void setLeftChild(Node * left_child);
	void setRightChild(Node * right_child);
	void setPriority(const int priority);
	void increasePriority();
	void setCode(const unsigned char code);

private:
	int priority;
	unsigned char code;
	Node * left_child;
	Node * right_child;
	
};

#endif // NODE_H