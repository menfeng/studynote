#include "Singleton.h"
#include <iostream>

using namespace std;

/*
 *Ensure a class only has one instance, and provide a global point of access to it. 
 *subclass of singleton:
 *	1.Use a env to decide which instance is created in Instance	
 *	2.Use a env to decide which instance is looked up in Instance(register the instances of subclass in constructor of subclass)
 */

int main(int argc,char* argv[])
{
	Singleton* singleton = Singleton::Instance();
	return 0;
}
