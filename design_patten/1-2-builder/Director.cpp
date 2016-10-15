#include "Director.h"
#include "Builder.h"
#include "Product.h"

Director::Director(){}
Director::~Director(){}

Product* Director::Construct1(Builder* builder)
{
	builder->CreateProduct();
	builder->BuildPartA("A1");
	builder->BuildPartB("B1");
	return builder->GetProduct();
}


Product* Director::ConstructComplex1(Builder* builder)
{
	builder->CreateProduct();
	builder->BuildPartA("AComplex1");
	builder->BuildPartB("BComplex1");
	return builder->GetProduct();
}

