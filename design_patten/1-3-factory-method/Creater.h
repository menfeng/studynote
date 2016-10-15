#ifndef _CREATER_H_
#define _CREATER_H_

class Product;

class Creater
{
public:
	virtual ~Creater();
	virtual Product* FactoryMethod();
	virtual Product* FactoryMethod(int id);
protected:
	Creater();
};

class Creater1:public Creater
{
public:
	Creater1();
	~Creater1();
	Product* FactoryMethod();
};

class Creater2:public Creater
{
public:
	Creater2();
	~Creater2();
	Product* FactoryMethod();
};

class CreaterParameterized:public Creater
{
public:
	CreaterParameterized();
	~CreaterParameterized();
	Product* FactoryMethod(int id);
};
#endif //_CREATER_H_
