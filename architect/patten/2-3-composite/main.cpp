#include "Component.h"
#include "Composite.h"
#include "Leaf.h"
#include <iostream>

using namespace std;

/*
 * Compose objects into tree structures to represent part-whole hierarchies.
 * Composite lets clients treat individual objects and compositions of objects uniformly
 */

int main(int argc,char* argv[])
{
	Leaf* lA = new Leaf("A");
	lA->Operation();

	Leaf* lB = new Leaf("B");
	lB->Operation();

	Composite* com = new Composite();
	com->Add(lA);
	com->Add(lB);
	com->Operation();

	Component* cA = com->GetChild(0);
	Component* cB = com->GetChild(1);
	cA->Operation();
	cB->Operation();

	com->Remove(cA);
	com->Operation();

	return 0;
}
