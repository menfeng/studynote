#include "Context.h"
#include "Strategy.h"

Context::Context(Strategy* stg)
{
	_stg = stg;
}

Context::~Context()
{
	delete _stg;
}

void Context::DoAction()
{
	_stg->Action();
}
