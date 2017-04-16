#include "Interpret.h"
#include <iostream>
using namespace std;

//Context
bool Context::Lookup(string k)
{
	map<string,bool>::iterator it=_map.find(k);
	return it->second;
}

void Context::Assign(string k,bool v)
{
	_map.insert(map<string,bool>::value_type(k,v));
}


//VariableExp
VariableExp::VariableExp(string str)
{
	_name=str;
}

string VariableExp::GetName()
{
	return _name;
}

bool VariableExp::Evaluate(Context& aContext)
{
	return aContext.Lookup(_name);
}

BooleanExp* VariableExp::Replace(string name, BooleanExp& exp)
{
	if (name==_name) {
		return exp.Copy();
	}else{
		return new VariableExp(_name);
	}
}

BooleanExp* VariableExp::Copy() const
{
	return new VariableExp(_name);
}

//AddExp
AndExp::AndExp (BooleanExp* op1, BooleanExp* op2)
{
	_operand1 = op1;
	_operand2 = op2;
}

bool AndExp::Evaluate (Context& aContext)
{
	return _operand1->Evaluate(aContext) && _operand2->Evaluate(aContext);
}

BooleanExp* AndExp::Copy () const
{ 
	return new AndExp(_operand1->Copy(), _operand2->Copy());
}

BooleanExp* AndExp::Replace (string name, BooleanExp& exp)
{
	return new AndExp( _operand1->Replace(name, exp), _operand2->Replace(name, exp));
}

//OrExp
OrExp::OrExp (BooleanExp* op1, BooleanExp* op2)
{
	_operand1 = op1;
	_operand2 = op2;
}

bool OrExp::Evaluate (Context& aContext)
{
	return _operand1->Evaluate(aContext) || _operand2->Evaluate(aContext);
}

BooleanExp* OrExp::Copy () const
{ 
	return new OrExp(_operand1->Copy(), _operand2->Copy());
}

BooleanExp* OrExp::Replace (string name, BooleanExp& exp)
{
	return new OrExp( _operand1->Replace(name, exp), _operand2->Replace(name, exp));
}

//NotExp
NotExp::NotExp (BooleanExp* op1)
{
	_operand1 = op1;
}

bool NotExp::Evaluate (Context& aContext)
{
	return !_operand1->Evaluate(aContext);
}

BooleanExp* NotExp::Copy () const
{ 
	return new NotExp(_operand1->Copy());
}

BooleanExp* NotExp::Replace (string name, BooleanExp& exp)
{
	return new NotExp( _operand1->Replace(name, exp));
}

//Constant
Constant::Constant (bool op1)
{
	_operand1 = op1;
}

bool Constant::Evaluate (Context& aContext)
{
	return _operand1;
}

BooleanExp* Constant::Copy () const
{ 
	return new Constant(_operand1);
}

BooleanExp* Constant::Replace (string name, BooleanExp& exp)
{
	return new Constant(_operand1);
}
