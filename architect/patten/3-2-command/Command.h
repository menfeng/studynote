#ifndef _COMMAND_H_
#define _COMMAND_H_

class Command{
public:
	virtual ~Command();
	virtual void Excute() = 0;
protected:
	Command();
};


template <class T>
class SimpleCommand:public Command{
public:
	typedef void (T::* Action)();
	SimpleCommand(T* rev,Action act);
	~SimpleCommand();
	virtual void Excute(void);
private:
	T* _rev;
	Action _act;
};

template <class T>
SimpleCommand<T>::SimpleCommand(T* rev,Action act)
{
	_rev = rev;
	_act = act;
}

template <class T>
SimpleCommand<T>::~SimpleCommand(){
	delete _rev;
}

template <class T>
void SimpleCommand<T>::Excute()
{
	(_rev->*_act)();
}

#endif //~_COMMAND_H_
