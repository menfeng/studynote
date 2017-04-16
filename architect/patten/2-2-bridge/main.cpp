#include "Abstraction.h"
#include "AbstractionImp.h"
#include <iostream>

using namespace std;

/*
 * Decouple an abstraction from its implementation so that the two can vary independently
 * Typically the Implementor interface provides only primitive operations, and Abstraction defines higher-level operations based on these primitives.
 */

int main(int argc,char* argv[])
{
	AbstractionImp* impA = new ConcreteAbstractionImpA();
	Abstraction* absA = new RefinedAbstraction(impA);
	AbstractionImp* impB = new ConcreteAbstractionImpB();
	Abstraction* absB = new RefinedAbstraction(impB);
	absA->Operation();
	absB->Operation();
	return 0;
}
