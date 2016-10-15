#ifndef _STRATEGY_H_
#define _STRATEGY_H_

class Strategy{
public:
	Strategy();
	virtual ~Strategy();
	virtual void Action() = 0;
};

class ConcreteStrategyA:public Strategy{
public:
	ConcreteStrategyA();
	virtual ~ConcreteStrategyA();
	void Action();
};

class ConcreteStrategyB:public Strategy{
public:
	ConcreteStrategyB();
	virtual ~ConcreteStrategyB();
	void Action();
};
#endif //~_STRATEGY_H_
