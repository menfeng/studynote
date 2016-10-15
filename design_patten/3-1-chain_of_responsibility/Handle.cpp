#include "Handle.h"
#include <iostream>
#include <string>
using namespace std;

Handle::Handle(Handle* successor,string handletext)
{
	this->_successor = successor;
	this->_handletext = handletext;
}
Handle::~Handle()
{
	delete _successor;
}

void Handle::HandleRequest()
{
	if(this->_successor!=0){
		this->_successor->HandleRequest();
	}
}


ConcreteHandleA::ConcreteHandleA(Handle* successor,string handletext)
	:Handle(successor,handletext){}
void ConcreteHandleA::HandleRequest()
{
	if(!this->_handletext.empty()){
		cout<<"ConcreteHandleA "<<this->_handletext<<endl;
	}
	else if(this->_successor!=0){
		cout<<"ConcreteHandleA trans to next"<<endl;
		this->_successor->HandleRequest();
	}
}

ConcreteHandleB::ConcreteHandleB(Handle* successor,string handletext)
	:Handle(successor,handletext){}

void ConcreteHandleB::HandleRequest()
{
	if(!this->_handletext.empty()){
		cout<<"ConcreteHandleB "<<this->_handletext<<endl;
	}
	else if(this->_successor!=0){
		cout<<"ConcreteHandleB trans to next"<<endl;
		this->_successor->HandleRequest();
	}
}
