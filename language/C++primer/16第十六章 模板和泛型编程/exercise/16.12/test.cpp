#include "stdafx.h"


//注意怎么找到迭代器所在容器的类型T::value_type
template<typename T> typename T::value_type  MostFreq( T first, T last ) 
{ 
	//  计算需分配内存的大小 
	std::size_t amount = 0; 
	T start = first; 
	while ( start != last ) 
	{ 
		amount++; 
		start++; 
	} 
	//  定义类型别名 
	typedef std::vector<typename T::value_type>   VecType; 
	//  创建vector 对象，用于保存输入序列的副本 
	VecType vec(amount); 
	VecType::iterator newFirst = vec.begin(); 
	VecType::iterator newLast = vec.end(); 
	//  将输入序列复制到vector 对象 
	std::uninitialized_copy( first, last, newFirst ); 
	std::sort ( newFirst, newLast );    //  对副本序列排序，使得相同的值出现在相邻位置 
		std::size_t maxOccu = 0, occu = 0;  //  出现最频繁的次数，当前值的出现次数 
		VecType::iterator preIter = newFirst;  // 指向当前值的前一个值 
		VecType::iterator maxOccuElement = newFirst;  // 指向当前出现最频繁的值 
		while( newFirst != newLast ) 
		{ 
			if  ( *newFirst != *preIter )    //  当前值与前一值不同 
			{ 
				if  ( occu > maxOccu )      //  当前值的出现次数为目前最大次数 
				{ 
					maxOccu = occu;              //修改最大次数 
						maxOccuElement = preIter;    //修改指向当前出现最频繁的值的迭代器 
				} 
				occu = 0;       
			} 
			++occu; 
			preIter = newFirst; 
			++newFirst; 
		} 
		//  最后一个值的出现次数与目前的最大次数进行比较 
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



