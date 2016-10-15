#include "Creater.h"

/*
 * Define an interface for creating an object, but let subclasses decide which class to instantiate. Factory Method lets a class defer instantiation to subclasses.
 */

int main(int argc,char* argv[])
{
	Creater* create1 = new Creater1();
	create1->FactoryMethod();

	Creater* create2 = new Creater2();
	create2->FactoryMethod();

	Creater* create3 = new CreaterParameterized();
	create3->FactoryMethod(1);
	create3->FactoryMethod(2);
	return 0;
}
