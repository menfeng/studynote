#include "Facade.h"
#include <iostream>
using namespace std;

/*
 *Provide a unified interface to a set of interfaces in a subsystem. 
 *Facade defines a higher-level interface that makes the subsystem easier to use.
 */

int main(int argc,char* argv[])
{
	Facade* f = new Facade();
	f->OperationWrapper();
	return 0;
}
