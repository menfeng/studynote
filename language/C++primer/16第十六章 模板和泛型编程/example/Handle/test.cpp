#include "stdafx.h"
#include "Handle.h"

// program takes single argument specifying the file to query
int main(int argc, char **argv) 
{
	{ // new scope
		// user allocates but must not delete the object to which the Handle is attached
		Handle<int> hp(new int(42));
		{ // new scope
			Handle<int> hp2 = hp; // copies pointer; use count incremented
			cout << *hp << " " << *hp2 << endl; // prints 42 42
			*hp2 = 10;           // changes value of shared underlying int
		}   // hp2 goes out of scope; use count is decremented
		cout << *hp << endl; // prints 10
	} // hp goes out of scope; its destructor deletes the int
	system("pause");
	return 0;
}



