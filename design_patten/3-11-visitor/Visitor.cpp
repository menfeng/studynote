#include "Visitor.h"
#include "Element.h"
#include <iostream>
using namespace std;
Visitor::Visitor(){}
Visitor::~Visitor(){}

ConcreteVisitorA::ConcreteVisitorA(){}
ConcreteVisitorA::~ConcreteVisitorA(){}
void ConcreteVisitorA::VisitConcreteElementA(Element* elm)
{
	cout<<"A visit..."<<endl;
}

void ConcreteVisitorA::VisitConcreteElementB(Element* elm)
{
	cout<<"A  visit..."<<endl;
}

ConcreteVisitorB::ConcreteVisitorB(){}
ConcreteVisitorB::~ConcreteVisitorB(){}
void ConcreteVisitorB::VisitConcreteElementA(Element* elm)
{
	cout<<"B visit..."<<endl;
}
void ConcreteVisitorB::VisitConcreteElementB(Element* elm)
{
	cout<<"B visit..."<<endl;
}
