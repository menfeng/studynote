#ifndef _BUILDER_H_
#define _BUILDER_H_

#include <string>
using namespace std;

class Product;

class Builder
{
public:
	virtual ~Builder();
	virtual void CreateProduct()=0;
	virtual void BuildPartA(const string& arg)=0;
	virtual void BuildPartB(const string& arg)=0;
	virtual Product* GetProduct()=0;
protected:
	Product* _product;

	Builder();
};

class Builder1:public Builder
{
public:
	Builder1();
	~Builder1();
	void CreateProduct();
	void BuildPartA(const string& arg);
	void BuildPartB(const string& arg);
	Product* GetProduct();
};

class Builder2:public Builder
{
public:
	Builder2();
	~Builder2();
	void CreateProduct();
	void BuildPartA(const string& arg);
	void BuildPartB(const string& arg);
	Product* GetProduct();
private:
	int _partA;
	int _partB;
};

#endif //_BUILDER_H_
