#ifndef _STATE_H_
#define _STATE_H_

class Context;
class State
{
	public:
		State();
		virtual ~State();
		virtual void Operation(Context* ) = 0;
		void ChangeState(Context*,State*);
};

class ConcreteStateA:public State
{
	public:
		ConcreteStateA();
		virtual ~ConcreteStateA();
		virtual void Operation(Context* );
		static ConcreteStateA* Instance();
	private:
		static ConcreteStateA* _instance;
};

class ConcreteStateB:public State
{
	public:
		ConcreteStateB();
		virtual ~ConcreteStateB();
		virtual void Operation(Context* );
		static ConcreteStateB* Instance();
	private:
		static ConcreteStateB* _instance;
};
#endif //~_STATE_H_
