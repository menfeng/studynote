#include "Strategy.h"
#include <iostream>
using namespace std;

Strategy::Strategy(){}
Strategy::~Strategy(){}
void Strategy::Action(){}

ConcreteStrategyA::ConcreteStrategyA(){}
ConcreteStrategyA::~ConcreteStrategyA()
{
	cout<<"~ConcreteStrategyA....."<<endl;
}
void ConcreteStrategyA::Action()
{
	cout<<"test ConcreteStrategyA....."<<endl;
}

ConcreteStrategyB::ConcreteStrategyB(){}
ConcreteStrategyB::~ConcreteStrategyB(){}
void ConcreteStrategyB::Action()
{
	cout<<"test ConcreteStrategyB....."<<endl;
}
