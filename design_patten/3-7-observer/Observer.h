#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <string>
#include <vector>
using namespace std;

class Observer;
class Subject
{
public:
	Subject();
	~Subject();
	void SetState(const string& sdt);
	string GetState();
	void Attach(Observer*);
	void Detach(Observer*);
	void Notify();
private:
	string _sdt;
	vector<Observer*> _obs;
};


class Observer 
{
public:
	Observer(Subject*);
	~Observer();
	void Update();
	void PrintState();

private:
	string _sdt;
	Subject* _sub;
};

#endif //~_OBSERVER_H_
