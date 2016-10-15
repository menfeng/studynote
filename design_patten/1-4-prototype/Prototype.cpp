#include "Prototype.h"
#include <iostream>
using namespace std;

Prototype::Prototype() { }
Prototype::~Prototype() { }
Prototype* Prototype::Clone() const { return 0; }

Prototype1::Prototype1(){}
Prototype1::~Prototype1(){}
Prototype1::Prototype1(const Prototype1& cp) 
{
	cout<<"Prototype1 copy ..."<<endl;
}
Prototype* Prototype1::Clone() const 
{ 
	return new Prototype1(*this);
}

Prototype2::Prototype2(){}
Prototype2::~Prototype2(){}
Prototype2::Prototype2(const Prototype2& cp) 
{
	cout<<"Prototype2 copy ..."<<endl;
}
Prototype* Prototype2::Clone() const 
{ 
	return new Prototype2(*this);
}
