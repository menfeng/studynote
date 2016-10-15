#include "Prototype.h"

/*
 * Specify the kinds of objects to create using a prototypical instance, and create new objects by copying this prototype.
 * In language like python ,class is a object which can be a prototype
 */

int main(int argc,char* argv[])
{
	Prototype* p1 = new Prototype1();
	Prototype* cp1 = p1->Clone();

	Prototype* p2 = new Prototype2();
	Prototype* cp2 = p2->Clone();
	return 0;
}
