#include "stdafx.h"


//ע����ô�ҵ���������������������T::value_type
template<typename T> typename T::value_type  MostFreq( T first, T last ) 
{ 
	//  ����������ڴ�Ĵ�С 
	std::size_t amount = 0; 
	T start = first; 
	while ( start != last ) 
	{ 
		amount++; 
		start++; 
	} 
	//  �������ͱ��� 
	typedef std::vector<typename T::value_type>   VecType; 
	//  ����vector �������ڱ����������еĸ��� 
	VecType vec(amount); 
	VecType::iterator newFirst = vec.begin(); 
	VecType::iterator newLast = vec.end(); 
	//  ���������и��Ƶ�vector ���� 
	std::uninitialized_copy( first, last, newFirst ); 
	std::sort ( newFirst, newLast );    //  �Ը�����������ʹ����ͬ��ֵ����������λ�� 
		std::size_t maxOccu = 0, occu = 0;  //  ������Ƶ���Ĵ�������ǰֵ�ĳ��ִ��� 
		VecType::iterator preIter = newFirst;  // ָ��ǰֵ��ǰһ��ֵ 
		VecType::iterator maxOccuElement = newFirst;  // ָ��ǰ������Ƶ����ֵ 
		while( newFirst != newLast ) 
		{ 
			if  ( *newFirst != *preIter )    //  ��ǰֵ��ǰһֵ��ͬ 
			{ 
				if  ( occu > maxOccu )      //  ��ǰֵ�ĳ��ִ���ΪĿǰ������ 
				{ 
					maxOccu = occu;              //�޸������� 
						maxOccuElement = preIter;    //�޸�ָ��ǰ������Ƶ����ֵ�ĵ����� 
				} 
				occu = 0;       
			} 
			++occu; 
			preIter = newFirst; 
			++newFirst; 
		} 
		//  ���һ��ֵ�ĳ��ִ�����Ŀǰ�����������бȽ� 
			if  ( occu > maxOccu ) 
			{ 
				maxOccu = occu; 
				maxOccuElement = preIter; 
			} 
			return  *maxOccuElement; 
}

// program takes single argument specifying the file to query
int main(int argc, char **argv)
{
	int a[]={1,1,2,2,2,3,5};
	vector<int> vec(a,a+5);
	cout<<MostFreq(vec.begin(),vec.end ());
	system("pause");
	return 0;
}



