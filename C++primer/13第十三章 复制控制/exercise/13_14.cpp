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

void  func1(Exmp obj)      //  形参为Exmp的对象 
{   
}   
void  func2(Exmp & obj)    //  形参为Exmp对象的引用 
{ 
} 
Exmp func3() 
{ 
	Exmp obj; 
	return  obj;                                        //  返回exmp对象 
} 

int _tmain( int argc, _TCHAR* argv[]) 
{ 
	{
		Exmp  exmp1;       //构造
		Exmp exmp2(exmp1); //复制构造
		exmp2 = exmp2;     //赋值
		func1( exmp1);     //复制构造，析构
		func2( exmp1);    //
		exmp1 = func3(); //构造，复制构造，析构，赋值，析构
		Exmp *p =  new  Exmp;      // 构造
		std::vector<Exmp> evec(3);//1构造，3复制构造 1析构 
		delete  p;               //析构
	}//5析构（3个为vector的，一个为exmp1，一个为exmp2）
	system("pause"); 
	return  0; 
} 



