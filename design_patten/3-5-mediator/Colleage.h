#ifndef _COLLEAGE_H_
#define _COLLEAGE_H_
#include <string>
using namespace std;

class Mediator;

class Colleage{
public:
	virtual ~Colleage();
	virtual void SetState(const string& sdt);
	virtual string GetState();
	virtual void ShowState()=0;
	virtual void Action()=0;
protected:
	Colleage();
	Colleage(Mediator* mdt);
	Mediator* _mdt;
	string _sdt;
};

class ConcreteColleageA:public Colleage{
public:
	ConcreteColleageA();
	ConcreteColleageA(Mediator* mdt);
	~ConcreteColleageA();
	virtual void Action();
	virtual void ShowState();
};

class ConcreteColleageB:public Colleage{
public:
	ConcreteColleageB();
	ConcreteColleageB(Mediator* mdt);
	~ConcreteColleageB();
	virtual void Action();
	virtual void ShowState();
};

#endif //_COLLEAGE_H_
