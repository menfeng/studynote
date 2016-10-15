#ifndef _DIRECTOR_H_
#define _DIRECTOR_H_

#include "Product.h"
#include "Builder.h"

class Builder;

class Director
{
public:
	Director();
	~Director();
	Product* Construct1(Builder * builder);
	Product* ConstructComplex1(Builder * builder);
};

#endif //~_DIRECTOR_H_
