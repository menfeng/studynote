#pragma once
#include "stdafx.h"
/*����
��1�������ʼ�����븳ֵ
��2����move���б߽���
��3�����д�ӡ��ʽ����
��4�����ǵ���const����Ĵ�ӡ*/
class  Screen 
{ 
public: 
	typedef std::string::size_type index; 
	Screen( index hei, index wid ,  const string &content ="")://size
	cursor(0),height(hei),width(wid)
	{	
		if (content.size ()!=0)
		{
			contents=content;
		}

		contents.resize(height*width);


	} 
	Screen & move(index r, index c) 
	{ 
		if (r>=height||c>=width)
		{
			cerr<<"invalid row or column"<<endl;
			throw EXIT_FAILURE;
		}
		index row = r * width; 
		cursor = row + c; 
		return  * this ; 
	} 
	Screen & set( char  c ) 
	{ 
		contents[cursor] = c; 
		return  * this ; 
	} 
	Screen & display( ostream &os ) 
	{ 
		do_display(os); 
		return  * this ; 
	} 
	const Screen & display(ostream &os ) const   
	{ 
		do_display(os); 
		return  * this ; 
	} 
private : 
	std::string contents; 
	index cursor; 
	index height, width; 
	void  do_display(ostream & os)  const 
	{   
		string::size_type index=0;
		while(index!=contents.size ())
		{
			os<<contents[index]<<" ";
			if ((index+1)%width==0)
			{
				cout<<'\n';
			}
			++index;
		}
		cout<<'\n';
	} 
}; 