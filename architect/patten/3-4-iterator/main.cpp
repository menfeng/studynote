/*
 * Provide a way to access the elements of an aggregate object sequentially without exposing its underlying representation
 * When the clientcontrols the iteration, the iterator is called an external iterator,and when the iterator controls it, the iterator is an internal iterator.
 */
#include "Iterator.h"
#include "Aggregate.h"
#include <iostream>
using namespace std;

int main(int argc,char* argv[])
{
	Aggregate* ag = new ConcreteAggregate();
	Iterator* it = new ConcreteIterator(ag);
	for (; !(it->IsDone()) ; it->Next())
	{
		cout<<it->CurrentItem()<<endl;
	}
	return 0;
}
