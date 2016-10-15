#include "Mediator.h"
#include "Colleage.h"

Mediator::Mediator(){}
Mediator::~Mediator(){}

ConcreteMediator::ConcreteMediator(){}
ConcreteMediator::~ConcreteMediator(){}
void ConcreteMediator::SetColleage(Colleage * clgA,Colleage* clgB)
{ 
	this->_clgA = clgA;
	this->_clgB = clgB;
}

void ConcreteMediator::DoActionFromA()
{
	_clgB->SetState(_clgA->GetState());
}
void ConcreteMediator::DoActionFromB()
{ 
	_clgA->SetState(_clgB->GetState());
}

