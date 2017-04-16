/*
* A program to transform words.
* Takes two arguments: The first is name of the word transformation file
*                      The second is name of the input to transform
*/
#include "stdafx.h"




class  Exmp 
{ 
public: 
	// constructor 
	Exmp() 
	{ 
		std::cout << " Using Exmp(). "  << std::endl; 
	} 
	//   
	// copy constructor 
	Exmp(const Exmp&) 
	{ 
		std::cout <<      " Using Exmp(const Exmp&) _copy constructor."  << std::endl; 
	} 
	//    overload operator  
	Exmp& operator =( const Exmp&) 
	{ 
		std::cout <<    " Using Exmp& operator=( const Exmp&) _overload operator."  << std::endl; 
		return  * this ; 
	} 

	// destructor 
	~Exmp() 
	{ 
		std::cout << " Using ~Exmp()." << std::endl; 
	} 

}; 

void  func1(Exmp obj)      //  �β�ΪExmp�Ķ��� 
{   
}   
void  func2(Exmp & obj)    //  �β�ΪExmp��������� 
{ 
} 
Exmp func3() 
{ 
	Exmp obj; 
	return  obj;                                        //  ����exmp���� 
} 

int _tmain( int argc, _TCHAR* argv[]) 
{ 
	{
		Exmp  exmp1;       //����
		Exmp exmp2(exmp1); //���ƹ���
		exmp2 = exmp2;     //��ֵ
		func1( exmp1);     //���ƹ��죬����
		func2( exmp1);    //
		exmp1 = func3(); //���죬���ƹ��죬��������ֵ������
		Exmp *p =  new  Exmp;      // ����
		std::vector<Exmp> evec(3);//1���죬3���ƹ��� 1���� 
		delete  p;               //����
	}//5������3��Ϊvector�ģ�һ��Ϊexmp1��һ��Ϊexmp2��
	system("pause"); 
	return  0; 
} 



