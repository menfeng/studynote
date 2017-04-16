/*
 * Represent an operation to be performed on the elements of an object structure.
 * Visitor lets you define a new operation without changing the classes of the elements on which it operates.
 * Double - dispatch
 *
 * Consequences:
 * 1. Visitor makes adding new operations easy
 * 2. A visitor gathers related operations and separates unrelated ones
 * 3. Adding new ConcreteElement classes is hard
 * 4. Visiting across class hierarchies,It can visit objects that don't have a common parent class
 * 5. Accumulating state.Visitors can accumulate state as they visit each element in the objectstructure.
 * 6. Breaking encapsulation
 */

#include "Element.h"
#include "Visitor.h"

int main(int argc,char* argv[])
{
	Visitor* va = new ConcreteVisitorA();
	Visitor* vb = new ConcreteVisitorB();
	Element* ea = new ConcreteElementA();
	Element* eb = new ConcreteElementB();
	ea->Accept(va);
	ea->Accept(vb);
	eb->Accept(va);
	eb->Accept(vb);
	return 0;
}
