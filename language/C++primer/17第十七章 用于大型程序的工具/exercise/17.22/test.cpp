#include "stdafx.h"
#include "MyClass.h"

namespace primerLib {
	void compute(){};
	void compute(const void *){cout<<"ff";};
}
//using primerLib::compute;  //(1)����compute(int)
void compute(int){cout<<"int";};
void compute(double, double = 3.4){};
void compute(char*, char* = 0){};

int main()
{   
	//using primerLib::compute; //(2)����compute(const void *)
	using namespace primerLib;  //(3)����(1)����compute(int)
	compute(0);
	system("pause");return 0;
}






