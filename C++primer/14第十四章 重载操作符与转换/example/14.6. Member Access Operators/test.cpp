
#include "stdafx.h"
#include "Screen.h"
#include "ScreenPtr.h"
int _tmain( int argc, _TCHAR* argv[]) 
{ 

	ScreenPtr myScreanPtr(new Screen(5,5,"fdddddfddfgdgerteryeyfg"));
	ScreenPtr myScreanPtr1(new Screen(5,5,"ffd"));
	myScreanPtr->display (cout);//��ͷ
	myScreanPtr1=myScreanPtr;//��ֵ
              (*myScreanPtr1).display (cout);//������
	system("pause"); 
	return  0; 
} 



