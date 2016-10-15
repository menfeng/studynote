#ifndef _HANDLE_H_
#define _HANDLE_H_

#include <string>
using namespace std;

class Handle{
public:
	Handle(Handle* successor,string handletext);
	virtual ~Handle();
	virtual void HandleRequest();
protected:
	Handle* _successor;
	string _handletext;
};

class ConcreteHandleA:public Handle{
public:
	ConcreteHandleA(Handle* successor,string handletext);
	void HandleRequest();
};

class ConcreteHandleB:public Handle{
public:
	ConcreteHandleB(Handle* successor,string handletext);
	void HandleRequest();
};

#endif //~_HANDLE_H_
