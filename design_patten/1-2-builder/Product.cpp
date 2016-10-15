#include <iostream>
#include "Product.h"

using namespace std;

Product::Product(){cout<<"Product..."<<endl;}
Product::~Product(){}

ProductPartA::ProductPartA(const string& arg){cout<<"ProductPartA "<<arg<<endl;}
ProductPartA::~ProductPartA(){}

ProductPartB::ProductPartB(const string& arg){cout<<"ProductPartB "<<arg<<endl;}
ProductPartB::~ProductPartB(){}

