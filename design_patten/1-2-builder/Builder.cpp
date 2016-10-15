#include "Builder.h"
#include "Product.h"
#include <string>
#include <iostream>
using namespace std;

Builder::Builder(){}
Builder::~Builder(){}

Builder1::Builder1(){_product=0;}
Builder1::~Builder1(){}
void Builder1::CreateProduct(){_product=new Product;}
Product* Builder1::GetProduct(){return _product;}
void Builder1::BuildPartA(const string& arg){new ProductPartA(arg);}
void Builder1::BuildPartB(const string& arg){new ProductPartB(arg);}

Builder2::Builder2(){_partA=_partB=0;}
Builder2::~Builder2(){}
void Builder2::CreateProduct(){}
Product* Builder2::GetProduct(){cout<<"_partA:"<<_partA<<" _partB:"<<_partB<<endl;return _product;}
void Builder2::BuildPartA(const string& arg){_partA++;}
void Builder2::BuildPartB(const string& arg){_partB++;}
