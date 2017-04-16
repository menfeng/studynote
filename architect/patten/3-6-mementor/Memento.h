#ifndef _MEMENTO_H_
#define _MEMENTO_H_
#include <string>
using namespace std;

class Memento;
class Originator
{
public:
	Originator();
	~Originator();
	void SetState(const string& sdt);
	Memento* CreateMemento();
	void SetMemento(Memento* men);
	void PrintState();
private:
	string _sdt;
	Memento* _mt;
};


class Memento
{
	private:
		friend class Originator;
		Memento();
		~Memento();
		void SetState(const string& sdt);
		string GetState();
	private:
		string _sdt;
};

#endif //~_MEMENTO_H_
