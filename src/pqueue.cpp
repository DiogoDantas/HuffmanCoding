#include "pqueue.h"

PQueue::PQueue(){}

PQueue::~PQueue(){}

void PQueue::insert(Node* node){
	prt_queue.push_back(node);
}

void PQueue::enqueue(Node* node){

	prt_queue.push_back(node);

	insertion_sort(prt_queue);
}

Node* PQueue::dequeue(){
	Node* tmp = prt_queue.back();
	prt_queue.pop_back();
	return tmp;

}

Node* PQueue::top(){

	return prt_queue.front();
}

void PQueue::build_heap(){

	heap_sort(prt_queue);
}

void PQueue::print_queue(){
	
	for (int i = 0; i < prt_queue.size(); ++i)
	{
		std::cout<<prt_queue.at(i)->getPriority()<< " "<<prt_queue.at(i)->getCode() << " ";
	}

	std::cout<<std::endl;
}

int PQueue::size(){
	return static_cast<int>(prt_queue.size());
}

void PQueue::insertion_sort(std::vector<Node*> &list)
{

	Node* tmp = 0;
	int i = 0, j = 0;

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

void PQueue::max_heapify( std::vector<Node*> &lista, const int pos, const int n ){
	int max = 2 * pos + 1;

    if( max < n )
    {
       if( (max+1) < n && lista.at(max)->getPriority() > lista.at(max+1)->getPriority() )
       {
          ++max;
       }
       if( lista.at(max)->getPriority() < lista.at(pos)->getPriority() )
       {
          std::swap( lista[max], lista[pos] );
          max_heapify( lista, max, n );
       }
    }
}

void PQueue::heap_sort( std::vector<Node*> &lista ){

	int tam = static_cast<int>( lista.size() ); 
    int i;

    for( i = tam/2 - 1; i >= 0; --i )
    {
       max_heapify(lista, i , tam );
    }

    std::vector<Node*>::reverse_iterator elem;

    for( elem = lista.rbegin(); elem != lista.rend(); elem++ )
    {
       std::iter_swap( elem, lista.begin() );
       max_heapify( lista, 0, --tam );
    }

}
