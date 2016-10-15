#include "Adapter.h"
#include <iostream>

using namespace std;

/*
 * Convert the interface of a class into another interface clients expect. Adapter
 * lets classes work together that couldn't otherwise because of incompatible interfaces.
 *	1.ClassAdapter : we can override some of adaptee's behavior
 *	2.ObjectAdapter: we can adapt a adaptee and all its subclasses
 */

int main(int argc,char* argv[])
{

	Target* classadapter = new ClassAdapter();
	classadapter->Request();

	Adaptee* adaptee = new Adaptee;
	Target* objectadapter = new ObjectAdapter(adaptee);
	objectadapter->Request();

	return 0;
}
