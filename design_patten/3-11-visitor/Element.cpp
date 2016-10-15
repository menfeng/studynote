#include "Element.h"
#include "Visitor.h"
#include <iostream>
using namespace std;

Element::Element(){}
Element::~Element(){}
void Element::Accept(Visitor* vis){}
ConcreteElementA::ConcreteElementA(){}
ConcreteElementA::~ConcreteElementA(){}

void ConcreteElementA::Accept(Visitor* vis)
{
	vis->VisitConcreteElementA(this);
	cout<<"A be visited "<<endl<<endl;
}

ConcreteElementB::ConcreteElementB(){}
ConcreteElementB::~ConcreteElementB(){}

void ConcreteElementB::Accept(Visitor* vis)
{
	vis->VisitConcreteElementB(this);
	cout<<"B be visited "<<endl<<endl;
}
