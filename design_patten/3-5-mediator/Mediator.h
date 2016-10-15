#ifndef _MEDIATOR_H_
#define _MEDIATOR_H_
class Colleage;

class Mediator{
public:
	virtual ~Mediator();
	virtual void DoActionFromA() = 0;
	virtual void DoActionFromB() = 0;
protected:
	Mediator();
};

class ConcreteMediator:public Mediator{
public:
	ConcreteMediator();
	~ConcreteMediator();
	void SetColleage(Colleage* clgA,Colleage* clgB);
	void DoActionFromA();
	void DoActionFromB();
private:
	Colleage* _clgA;
	Colleage* _clgB;
};
#endif //~_MEDIATOR_H
