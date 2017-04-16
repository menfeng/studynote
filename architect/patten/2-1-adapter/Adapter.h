#ifndef _ADAPTER_H_
#define _ADAPTER_H_

class Target
{
public:
	Target();
	virtual ~Target();
	virtual void Request();
};

class Adaptee
{
public:
	Adaptee();
	~Adaptee();
	void SpecificRequest();
};

class ClassAdapter:public Target,private Adaptee
{
public:
	ClassAdapter();
	~ClassAdapter();
	void Request();
};


class ObjectAdapter:public Target
{
public:
	ObjectAdapter(Adaptee* adaptee);
	~ObjectAdapter();
	void Request();
private:
	Adaptee* _adaptee;
};

#endif //~_ADAPTER_H_
