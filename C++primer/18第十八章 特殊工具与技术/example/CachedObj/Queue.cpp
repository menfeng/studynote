#include "stdafx.h"
#ifndef CPP
#define CPP

#include "Queue.h"

template <class Type>   ostream& operator<<(ostream &os, const Queue<Type> &q)
{
	os << "< ";
	QueueItem<Type> *p;
	for (p = q.head; p; p = p->next)
		os << p->item << " ";
	os <<">";
	return os;
}

template <class Type> void Queue<Type>::copy_elems(const Queue &orig)
{
	// copy elements from orig into this Queue
	// loop stops when pt == 0, which happens when we reach orig.tail
	for (QueueItem<Type> *pt = orig.head; pt; pt = pt->next)
		push(pt->item); // copy the element
}

template <class Type> template <class It>  void Queue<Type>::copy_elems(It beg, It end)
{
	while (beg != end) {
		push(*beg);
		++beg;
	}
}

template <class Type>  Queue<Type>& Queue<Type>::operator = (const Queue<Type>& orig )
{
	destroy ();
	copy_elems (orig);
	return *this;
}

template <class Type> void Queue<Type>::destroy()
{
	while (!empty())
		pop();
}

template <class Type> void Queue<Type>::pop()
{
	QueueItem<Type>* p = head; 
	head = head->next;        
	delete p;                  
}

template <class T> template <class Iter>  void Queue<T>::assign(Iter beg, Iter end)
{
	destroy();           
	copy_elems(beg, end); 
}

template <class Type> void Queue<Type>::push(const Type &val)
{
	
	QueueItem<Type> *pt = new QueueItem<Type>(val);
	if (empty())
		head = tail = pt; 
	else 
	{
		tail->next = pt; 
		tail = pt;
	}
}

#endif







