/*
 * Define an object that encapsulates how a set of objects interact.
 * Mediator promotes loose coupling by keeping objects from referring to each other explicitly, 
 * and it lets you vary their interactionindependently.
 */

#include "Mediator.h"
#include "Colleage.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[])
{
	ConcreteMediator* m = new ConcreteMediator();
	ConcreteColleageA* c1 = new ConcreteColleageA(m);
	ConcreteColleageB* c2 = new ConcreteColleageB(m);
	m->SetColleage(c1,c2);

	c1->SetState("old");
	c2->SetState("old");
	c1->ShowState();
	c2->ShowState();

	c1->SetState("new");
	c1->ShowState();
	c2->ShowState();

	c1->Action();
	c1->ShowState();
	c2->ShowState();
	return 0;
}
