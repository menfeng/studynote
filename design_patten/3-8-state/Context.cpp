#include "Context.h"
#include "State.h"
Context::Context() { }

Context::Context(State* state)
{
	this->_state = state;
}

Context::~Context()
{
	delete _state;
}

void Context::Operation()
{
	_state->Operation(this);
}

void Context::ChangeState(State* state)
{
	this->_state = state;
}

