/*
 * Without violating encapsulation, capture and externalize an object'sinternal state so that the object can be restored to this state later
 */

#include "Memento.h"
#include <iostream>
using namespace std;

int main(int argc,char* argv[])
{
	Originator* o = new Originator();
	o->SetState("old");
	o->PrintState();
	Memento* m = o->CreateMemento();
	o->SetState("new");
	o->PrintState();
	o->SetMemento(m);
	o->PrintState();
	return 0;
}
