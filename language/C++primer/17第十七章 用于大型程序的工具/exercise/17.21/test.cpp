#include "stdafx.h"
#include "MyClass.h"

namespace Exercise 
{int ivar = 0;double dvar = 0;const int limit = 1000;}
int ivar = 10;
/*
using Exercise::ivar;//���������
using Exercise::dvar;  
using Exercise::limit;
*/
/*using namespace Exercise;*/  //������ֶ�������ԣ�����++ivar;������
int main(int argc, char **argv) 
{   /*
	using Exercise::ivar;   //++ivarʹ�õ���Exercise::ivar
	using Exercise::dvar;   //���������  
	using Exercise::limit;*/
    using namespace Exercise;////�൱��������ȫ�ֿռ䣬������ֶ�������ԣ�����c++ivar;������
	double dvar = 3.1416;
	cout<<dvar<<endl;
	int iobj = limit + 1;
	cout<<iobj<<endl
		<<++ivar<<endl
		<<++::ivar<<endl;
	system("pause");return 0;
}
//����ֻ����һ�����֣��������õ�using���ֵĵط�
//ָʾ�������������֣����������뵽��Χ���������鷢����ʹ�õĵط������������ĵط�




