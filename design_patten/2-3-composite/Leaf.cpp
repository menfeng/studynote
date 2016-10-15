#include "Leaf.h"
#include <iostream>
#include <string>

using namespace std;

Leaf::Leaf(const char* name){_name=string(name);}

Leaf::~Leaf(){}

void Leaf::Operation()
{
	cout<<"Leaf operation....."<<_name<<endl;
}
