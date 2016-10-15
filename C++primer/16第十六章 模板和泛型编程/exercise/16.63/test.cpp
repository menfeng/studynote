#include "stdafx.h"

template <class T> void calc(T, T)
{
   cout<<"calc(T, T)"<<endl;
}
void calc(double, double)
{
    cout<<"calc(double, double)"<<endl;
}
template <> void calc<char>(char, char)
{
    cout<<"calc<char>(char, char)"<<endl;
}

int main(int argc, char **argv) 
{

	int ival=0; 
	const int cival=0;
	double dval=0; 
	float fd=0;
	calc(0, ival);     //T
	calc (ival,cival);//T非引用的形参，实参的const会被丢掉
	calc(0.25, dval);//double
	calc(0, fd);    //double  
	calc (0, 'J');//double

	system("pause");
	return 0;
}




