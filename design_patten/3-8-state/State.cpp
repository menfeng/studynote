#include "State.h"
#include "Context.h"
#include <iostream>
using namespace std;

State::State(){}

State::~State(){}

void State::Operation(Context* con)
{
	cout<<"State::.."<<endl;
}

void State::ChangeState(Context* con,State* st)
{
	con->ChangeState(st);
}

ConcreteStateA* ConcreteStateA::_instance=0;
ConcreteStateA::ConcreteStateA(){}
ConcreteStateA::~ConcreteStateA(){}
ConcreteStateA* ConcreteStateA::Instance(){
	if(_instance == 0)_instance = new ConcreteStateA();
	return _instance;
}

void ConcreteStateA::Operation(Context* con)
{
	cout<<"ConcreteStateA::Operation......"<<endl;
	ChangeState(con,ConcreteStateB::Instance());
}

ConcreteStateB* ConcreteStateB::_instance=0;
ConcreteStateB::ConcreteStateB(){}
ConcreteStateB::~ConcreteStateB(){}
ConcreteStateB* ConcreteStateB::Instance(){
	if(_instance == 0)_instance = new ConcreteStateB();
	return _instance;
}
void ConcreteStateB::Operation(Context* con)
{
	cout<<"ConcreteStateB::Operation......"<<endl;
	ChangeState(con,ConcreteStateA::Instance());
}
