#include "Product.h"
#include <iostream>

using namespace std;


AbstractProductA::AbstractProductA(){}

AbstractProductA::~AbstractProductA(){}

ProductA1::ProductA1(){cout<<"ProductA1..."<<endl;}

ProductA1::~ProductA1(){}

ProductA2::ProductA2(){cout<<"ProductA2..."<<endl;}

ProductA2::~ProductA2(){}


AbstractProductB::AbstractProductB(){}

AbstractProductB::~AbstractProductB(){}

ProductB1::ProductB1(){cout<<"ProductB1..."<<endl;}

ProductB1::~ProductB1(){}

ProductB2::ProductB2(){cout<<"ProductB2..."<<endl;}

ProductB2::~ProductB2(){}
