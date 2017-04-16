#include "Adapter.h"
#include <iostream>
using namespace std;

Target::Target(){}
Target::~Target(){}
void Target::Request()
{
	cout<<"Target::Request"<<endl;
}

Adaptee::Adaptee(){}
Adaptee::~Adaptee(){}
void Adaptee::SpecificRequest()
{
	cout<<"Adaptee::SpecificRequest"<<endl;
}

ClassAdapter::ClassAdapter(){}
ClassAdapter::~ClassAdapter(){}
void ClassAdapter::Request()
{
	this->SpecificRequest();
}


ObjectAdapter::ObjectAdapter(Adaptee* adaptee){this->_adaptee = adaptee;}
ObjectAdapter::~ObjectAdapter(){}
void ObjectAdapter::Request()
{
	_adaptee->SpecificRequest();
}
