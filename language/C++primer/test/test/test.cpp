#include "stdafx.h"

class A { public: virtual ~A(){}};
class B : public  A {       };
class C : public  B { /* ... */ };



int main(int argc, char **argv) 
{
	

	//C cobj;
	//A& ra = cobj;
	//cout << typeid(&ra).name() << endl;

	B b;
	A& ra = b;
	cout << typeid(ra).name() << endl;
	system("pause");
	return 0;
}



