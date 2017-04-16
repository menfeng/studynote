#include "Proxy.h"
#include <iostream>
using namespace std;

/*
 *Provide a surrogate or placeholder for another object to control access to it.
 *Remote Proxy, Virtual Proxy, Protection Proxies, Smart Reference
 */

int main(int argc,char* argv[])
{
	Subject* sub = new ConcreteSubject();
	Proxy* p = new Proxy(sub);
	p->Request();
	return 0;
}
