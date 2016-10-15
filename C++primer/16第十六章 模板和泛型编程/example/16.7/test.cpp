#include "stdafx.h"

//A
//template <typename T> int compare(const T&, const T&)
//{
//	cout<<"A\n";
//	return 0;
//}
//T&
template <typename T> int compare(T&, T&)
{
	cout<<"T&\n";
	return 0;
}
//
////B
//template <typename T> int compare(T, T)
//{
//	cout<<"B\n";
//	return 0;
//}
//
////C
//int compare(const char*, const char*)
//{
//	cout<<"C\n";
//	return 0;
//}
////D
//int compare(char*, char*)
//{
//	cout<<"D\n";
//	return 0;
//}



int main(int argc, char **argv) 
{

	const char const_arr1[] = "world", const_arr2[] = "hihhh";
	char ch_arr1[] = "world", ch_arr2[] = "hihhh";

	const char *cp1 = const_arr1, *cp2 = const_arr2;
	char *p1 = ch_arr1, *p2 = ch_arr2;
                                 //数组大小不同----------------------------------------大小相同                                         
	                             //BCD    ACD    ABD  ABC ABCD     AB        A    B     AT&  BT&   AB   A   T&
	compare(const_arr1, const_arr2);//C       C     B     C     C      B      er2     B     A    er1  er1   A
	compare(ch_arr1, ch_arr2);     //D       D     D     B     D       B      er2    B     T&    er1  er1  A
	compare(cp1, cp2);            //C       C     er1   C     C      er1      A     B     T&    er1  er1  A
	compare(p1, p2);              //D       D     D     er1   D      er1      A     B     T&    er1  er1  A


	system("pause");
	return 0;
}

//er1: “compare”: 对重载函数的调用不明确	
//er2: “int compare(const T &,const T &)”: 模板 参数“T”不明确
/*
 总结：
  （1）由B,A 对于非引用的模版，数组可转换为指针，但由BCD，ACD，ABCD数组更倾向于对应的指针
  （2）由ABD const数组在没有对应的指针时，会选择模版T；（他不能选择const T&  因为对于引用数组不转指针）
             const指针在没有对应的指针时，在模版T和模版const T&中二义性
		     由ABC此原理使用于普通数组普通指针
  （3）由（2）可解释AB
  （4）由A不同大小的数组，传给（T&，T&）、（const T&，const T&）模版会出错的,若大小相同则不会
   (5) 由AT&常量用常量引用，非常亮用非常量引用（const char *cp1 也是一个非常量）
   
 */



