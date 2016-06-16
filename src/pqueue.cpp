#include "pqueue.h"

PQueue::PQueue(){}

PQueue::~PQueue(){}

void PQueue::enqueue(Node* node){
	if(node->getPriority() == 0) return;
	prt_queue.push_back(node);

	insertion_sort(prt_queue);
}

Node* PQueue::dequeue(){
	Node* tmp = prt_queue.back();
	prt_queue.pop_back();
	return tmp;
}

Node* PQueue::top() const{

	return prt_queue.front();
}


void PQueue::print_queue() const{
	
	for (unsigned int i = 0; i < prt_queue.size(); ++i)
	{
		std::cout<<prt_queue.at(i)->getPriority()<< " "<<prt_queue.at(i)->getCode() << " ";
	}

	std::cout<<std::endl;
}

int PQueue::size() const {

	return static_cast<int>(prt_queue.size());
}

void PQueue::insertion_sort(std::vector<Node*> &list)
{

	Node* tmp = 0;
	unsigned int i = 0;
	int j = 0;

	for (i = 1; i < list.size(); ++i)
	{
		tmp = list[i];
		for (j = i-1; j >= 0 && list[j]->getPriority() < tmp->getPriority(); --j)
		{
			list[j+1] = list[j];
		}

		list[j+1] = tmp;
	}
}


