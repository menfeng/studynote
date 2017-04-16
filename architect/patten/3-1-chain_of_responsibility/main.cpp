/*
 * Avoid coupling the sender of a request to its receiver by giving morethan one object a chance to handle the request. 
 * Chain the receiving objects and pass the request along the chain until an object handles it.
 */

#include "Handle.h"
#include <iostream>
using namespace std;

int main(int argc,char* argv[])
{
	Handle* h1 = new ConcreteHandleA(0,"h1");
	Handle* h2 = new ConcreteHandleB(h1,"h2");
	Handle* h3 = new ConcreteHandleB(h1,"");
	h2->HandleRequest();
	h3->HandleRequest();
	return 0;
}
