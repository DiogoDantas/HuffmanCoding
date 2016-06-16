#ifndef P_QUEUE_H
#define P_QUEUE_H

#include <vector>
#include "node.h"

class PQueue
{
public:
	PQueue();
	~PQueue();
	void enqueue(Node* node);
	Node* dequeue();
	Node* top() const;
	int size() const ;
	void print_queue() const;


private:
	void insertion_sort(std::vector<Node*> &lista);
	std::vector<Node*> prt_queue;
};
#endif // P_QUEUE_H