#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include <string>

using namespace std;

class Product
{
public:
	Product();
	~Product();
};

class ProductPartA
{
public:
	ProductPartA(const string& arg);
	~ProductPartA();
};

class ProductPartB
{
public:
	ProductPartB(const string& arg);
	~ProductPartB();
};

#endif //_PRODUCT_H_
