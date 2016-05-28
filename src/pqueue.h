#ifndef P_QUEUE_H
#define P_QUEUE_H

#include <vector>
#include "node.h"

class PQueue
{
public:
	PQueue();
	~PQueue();
	void insert(Node* node);
	void enqueue(Node* node);
	Node* dequeue();
	Node* top() const;
	//void build_heap();	
	int size() const ;
	void print_queue() const;


private:
	//void max_heapify( std::vector<Node*> &lista, const int pos, const int n );
	//void heap_sort( std::vector<Node*> &lista );
	void insertion_sort(std::vector<Node*> &lista);
	std::vector<Node*> prt_queue;
};
#endif // P_QUEUE_H