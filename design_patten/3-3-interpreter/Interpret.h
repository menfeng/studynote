#ifndef _INTERPRET_H_
#define _INTERPRET_H_

#include <string>
#include <map>
using namespace std;

class Context {
public:
	bool Lookup(string);
	void Assign(string, bool);
private:
	map<string ,bool> _map;
};

class BooleanExp {
public:
	virtual bool Evaluate(Context&) = 0;
	virtual BooleanExp* Replace(string, BooleanExp&) = 0;
	virtual BooleanExp* Copy() const = 0;
};

class VariableExp : public BooleanExp {
public:
	VariableExp(string);
	string GetName();
	virtual bool Evaluate(Context&);
	virtual BooleanExp* Replace(string, BooleanExp&);
	virtual BooleanExp* Copy() const;
private:
	string _name;
};

class AndExp : public BooleanExp{
public:
	AndExp(BooleanExp*, BooleanExp*);
	virtual bool Evaluate(Context&);
	virtual BooleanExp* Replace(string, BooleanExp&);
	virtual BooleanExp* Copy() const;
private:
	BooleanExp* _operand1;
	BooleanExp* _operand2;
};

class OrExp : public BooleanExp{
public:
	OrExp(BooleanExp*, BooleanExp*);
	virtual bool Evaluate(Context&);
	virtual BooleanExp* Replace(string, BooleanExp&);
	virtual BooleanExp* Copy() const;
private:
	BooleanExp* _operand1;
	BooleanExp* _operand2;
};

class NotExp : public BooleanExp{
public:
	NotExp(BooleanExp*);
	virtual bool Evaluate(Context&);
	virtual BooleanExp* Replace(string, BooleanExp&);
	virtual BooleanExp* Copy() const;
private:
	BooleanExp* _operand1;
};

class Constant : public BooleanExp{
public:
	Constant(bool);
	virtual bool Evaluate(Context&);
	virtual BooleanExp* Replace(string, BooleanExp&);
	virtual BooleanExp* Copy() const;
private:
	bool _operand1;
};

#endif //~_INTERPRET_H_
