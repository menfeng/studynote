/*
 * Allow an object to alter its behavior when its internal state changes.The object will appear to change its class.
 */

#include "Context.h"
#include "State.h"

int main(int argc,char* argv[])
{
	Context* con = new Context(ConcreteStateA::Instance());
	con->Operation();
	con->Operation();
	con->Operation();
	return 0;
}
