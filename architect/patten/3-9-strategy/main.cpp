/*
 * Define a family of algorithms, encapsulate each one, and make them interchangeable.
 * Strategy lets the algorithm vary independently from clients that use it.
 */

#include "Context.h"
#include "Strategy.h"

int main(int argc,char* argv[])
{
	Context* pcA = new Context(new ConcreteStrategyA());
	Context* pcB = new Context(new ConcreteStrategyB());
	pcA->DoAction();
	pcB->DoAction();
	return 0;
}
