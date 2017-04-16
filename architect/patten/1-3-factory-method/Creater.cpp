#include "Creater.h"
#include "Product.h"

Creater::Creater(){}
Creater::~Creater(){}
Product* Creater::FactoryMethod(){return new Product;}
Product* Creater::FactoryMethod(int id){return new Product;}

Creater1::Creater1(){}
Creater1::~Creater1(){}
Product* Creater1::FactoryMethod(){return new Product1;}

Creater2::Creater2(){}
Creater2::~Creater2(){}
Product* Creater2::FactoryMethod(){return new Product2;}

CreaterParameterized::CreaterParameterized(){}
CreaterParameterized::~CreaterParameterized(){}
Product* CreaterParameterized::FactoryMethod(int id)
{
	if(id==1){
		return new Product1;
	}
	if(id==2){
		return new Product2;
	}
	return new Product;
}
