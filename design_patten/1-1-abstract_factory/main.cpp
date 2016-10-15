#include "AbstractFactory.h"

/*
 * Provide an interface for creating families of related or dependent objects without specifying their concrete classes
 */

int main(int argc,char* argv[])
{
	AbstractFactory* cf1 = new ConcreteFactory1();
	cf1->CreateProductA();
	cf1->CreateProductB();
	AbstractFactory* cf2 = new ConcreteFactory2();
	cf2->CreateProductA();
	cf2->CreateProductB();
	return 0;
}
