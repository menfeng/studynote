#include "Decorator.h"
#include <iostream>
using namespace std;

/*
 * Attach additional responsibilities to an object dynamically. 
 * Decorators provide a flexible alternative to subclassing for extending functionality.
 */

int main(int argc,char* argv[])
{
	Component* com = new ConcreteComponent();
	Decorator* dec = new ConcreteDecorator(com);
	dec->Operation();
	return 0;
}
