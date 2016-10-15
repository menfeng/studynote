#include "stdafx.h"
#include "MyClass.h"

namespace Exercise 
{int ivar = 0;double dvar = 0;const int limit = 1000;}
int ivar = 10;
/*
using Exercise::ivar;//定义二义性
using Exercise::dvar;  
using Exercise::limit;
*/
/*using namespace Exercise;*/  //不会出现定义二义性，但是++ivar;二义性
int main(int argc, char **argv) 
{   /*
	using Exercise::ivar;   //++ivar使用的是Exercise::ivar
	using Exercise::dvar;   //定义二义性  
	using Exercise::limit;*/
    using namespace Exercise;////相当于引入了全局空间，不会出现定义二义性，但是c++ivar;二义性
	double dvar = 3.1416;
	cout<<dvar<<endl;
	int iobj = limit + 1;
	cout<<iobj<<endl
		<<++ivar<<endl
		<<++::ivar<<endl;
	system("pause");return 0;
}
//声明只引入一个名字，并就引用到using出现的地方
//指示会引入所有名字，并将其引入到外围，但错误检查发生在使用的地方而不是声明的地方




