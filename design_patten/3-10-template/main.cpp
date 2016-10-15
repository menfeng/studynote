/*
 * Define the skeleton of an algorithm in an operation, deferring somesteps to subclasses. 
 * Template Method lets subclasses redefine certain steps of an algorithm without changing the algorithm's structure.
 */

#include "Template.h"

int main(int argc,char* argv[])
{
	AbstractClass* p1 = new ConcreteClass1();
	AbstractClass* p2 = new ConcreteClass2();
	p1->TemplateMethod();
	p2->TemplateMethod();
	return 0;
}
