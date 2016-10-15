#include "Memento.h"
#include <iostream>
using namespace std;
typedef string State;

Originator::Originator(){
	_sdt = "";
	_mt = 0;
}

Originator::~Originator(){}

void Originator::SetState(const State& sdt){
	_sdt = sdt;
}

Memento* Originator::CreateMemento(){
	Memento* p= new Memento();
	p->SetState(_sdt);
	return p;
}

void Originator::PrintState(){
	cout<<this->_sdt<<"....."<<endl;
}

void Originator::SetMemento(Memento* mt){
	this->_sdt = mt->GetState();
}



Memento::Memento(){
	_sdt="";
}

Memento::~Memento(){}

State Memento::GetState(){return _sdt;}

void Memento::SetState(const State& sdt){
	_sdt = sdt;
}
