
#include "stdafx.h"
#include "Screen.h"
#include "ScreenPtr.h"
int _tmain( int argc, _TCHAR* argv[]) 
{ 

	ScreenPtr myScreanPtr(new Screen(5,5,"fdddddfddfgdgerteryeyfg"));
	ScreenPtr myScreanPtr1(new Screen(5,5,"ffd"));
	myScreanPtr->display (cout);//箭头
	myScreanPtr1=myScreanPtr;//赋值
              (*myScreanPtr1).display (cout);//解引用
	system("pause"); 
	return  0; 
} 



