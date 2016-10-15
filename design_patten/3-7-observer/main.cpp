/*
 * Define a one-to-many dependency between objects so that when one object changes state, all its dependents are notified and updated automatically.
 */

#include "Observer.h"
#include <iostream>
using namespace std;

int main(int argc,char* argv[])
{
	Subject* s = new Subject();
	Observer* o=new Observer(s);
	s->SetState("old");
	s->Notify();
	o->PrintState();
	s->SetState("new");
	s->Notify();
	o->PrintState();
	return 0;
}
