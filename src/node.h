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
	void setLeftChild(Node *left_child);
	void setRightChild(Node *right_child);
	void setPriority(const int priority);
	void increasePriority();
	void decreasePriority();
	void setCode(const unsigned char code);

private:
	int priority = 0;
	unsigned char code = 0;
	Node *left_child = nullptr;
	Node *right_child = nullptr;
	
};

#endif // NODE_H