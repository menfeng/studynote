#include "Mediator.h"
#include "Colleage.h"
#include <iostream>
using namespace std;

Colleage::Colleage(){}
Colleage::Colleage(Mediator* mdt){this->_mdt=mdt;}
Colleage::~Colleage(){}
string Colleage::GetState(){ return _sdt;}
void Colleage::SetState(const string& sdt){ _sdt=sdt;}


ConcreteColleageA::ConcreteColleageA(){}
ConcreteColleageA::~ConcreteColleageA(){}
ConcreteColleageA::ConcreteColleageA(Mediator* mdt):Colleage(mdt){}
void ConcreteColleageA::Action()
{
	_mdt->DoActionFromA();
}
void ConcreteColleageA::ShowState()
{
	cout<<"State of ConcreteColleageA:"<<" "<<this->GetState()<<endl;
}


ConcreteColleageB::ConcreteColleageB(){}
ConcreteColleageB::~ConcreteColleageB(){}
ConcreteColleageB::ConcreteColleageB(Mediator* mdt):Colleage(mdt){}
void ConcreteColleageB::Action()
{
	_mdt->DoActionFromB();
}
void ConcreteColleageB::ShowState()
{
	cout<<"State of ConcreteColleageB:"<<" "<<this->GetState()<<endl;
}

