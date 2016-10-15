/*
 * Given a language, define a represention for its grammar along with an interpreter
 * that uses the representation to interpret sentences in the language.
 *
 * example
 * BooleanExp ::= VariableExp | Constant | OrExp | AndExp | NotExp | '(' BooleanExp ')'
 * AndExp ::= BooleanExp 'and' BooleanExp
 * OrExp ::= BooleanExp 'or' BooleanExp
 * NotExp ::= 'not' BooleanExp
 * Constant ::= 'true' | 'false'
 * VariableExp ::= 'A' | 'B' | ... | 'X' | 'Y' | 'Z'
 */

#include "Interpret.h"
#include <iostream>
using namespace std;

int main(int argc,char* argv[])
{

	bool result;
	VariableExp* x = new VariableExp("X");
	VariableExp* y = new VariableExp("Y");
	VariableExp* z = new VariableExp("Z");

	//(true and x) or (y and (not x))
	BooleanExp* expression;
	expression = new OrExp(
			new AndExp(new Constant(true), x),
			new AndExp(y, new NotExp(x)));

	//context
	Context context;
	context.Assign("X", false);
	context.Assign("Y", true);
	context.Assign("Z", true);
	result=expression->Evaluate(context);
	//1&&0 || 1&&1
	cout<<result<<endl;

	NotExp not_z(z);
	BooleanExp* replacement = expression->Replace("Y", not_z);
	result = replacement->Evaluate(context);
	//1&&0 || 0&&1
	cout<<result<<endl;

	return 0;
}
