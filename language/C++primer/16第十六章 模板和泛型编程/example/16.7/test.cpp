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
                                 //�����С��ͬ----------------------------------------��С��ͬ                                         
	                             //BCD    ACD    ABD  ABC ABCD     AB        A    B     AT&  BT&   AB   A   T&
	compare(const_arr1, const_arr2);//C       C     B     C     C      B      er2     B     A    er1  er1   A
	compare(ch_arr1, ch_arr2);     //D       D     D     B     D       B      er2    B     T&    er1  er1  A
	compare(cp1, cp2);            //C       C     er1   C     C      er1      A     B     T&    er1  er1  A
	compare(p1, p2);              //D       D     D     er1   D      er1      A     B     T&    er1  er1  A


	system("pause");
	return 0;
}

//er1: ��compare��: �����غ����ĵ��ò���ȷ	
//er2: ��int compare(const T &,const T &)��: ģ�� ������T������ȷ
/*
 �ܽ᣺
  ��1����B,A ���ڷ����õ�ģ�棬�����ת��Ϊָ�룬����BCD��ACD��ABCD����������ڶ�Ӧ��ָ��
  ��2����ABD const������û�ж�Ӧ��ָ��ʱ����ѡ��ģ��T����������ѡ��const T&  ��Ϊ�����������鲻תָ�룩
             constָ����û�ж�Ӧ��ָ��ʱ����ģ��T��ģ��const T&�ж�����
		     ��ABC��ԭ��ʹ������ͨ������ָͨ��
  ��3���ɣ�2���ɽ���AB
  ��4����A��ͬ��С�����飬������T&��T&������const T&��const T&��ģ�������,����С��ͬ�򲻻�
   (5) ��AT&�����ó������ã��ǳ����÷ǳ������ã�const char *cp1 Ҳ��һ���ǳ�����
   
 */



