#include "stdafx.h"

template <typename T> void compare(const T& v1, const T& v2){cout<<"const T&"<<endl;}
/************************************************************************************/

template <typename T> void fobj(T, T){cout<<"fobj(T, T)"<<endl;}
template <typename T> void fref(const T&, const T&){cout<<"fref(const T&, const T&)"<<endl;}

int main(int argc, char **argv) 
{
	short si;
	//compare(si, 1024);//错误两个参数类型需要相同

	string s1("a value");
	const string s2("another value");
	fobj(s1, s2);     // ok: calls f(string, string), const is ignored
	fref(s1, s2);     // ok: non const object s1 converted to const reference
	int a[10], b[42];
	fobj(a, b); // ok: calls f(int*, int*)
	//fref(a, b); // error: array types don't match; arguments aren't converted to pointers



	system("pause");
	return 0;
}




