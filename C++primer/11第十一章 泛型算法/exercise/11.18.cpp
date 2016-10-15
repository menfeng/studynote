/*
* A program to transform words.
* Takes two arguments: The first is name of the word transformation file
*                      The second is name of the input to transform
*/
#include "stdafx.h"




int _tmain( int argc, _TCHAR* argv[]) 
{ 
	
	ofstream of1;
	ofstream of2;
	 of1.open("1.txt");
	 of2.open("2.txt");
	if  ( !of1 || !of2    ) 
	{ 
		cerr << " File can't be open. "  << endl; 
		return  -1; 
	} 


   istream_iterator<int> cin_it(cin),end;    // reads ints from cin

   ostream_iterator<int> out1(of1," ");
   ostream_iterator<int> out2(of2,"\n");
    while(cin_it != end)
	{
		if (*cin_it%2==0)
		{
			*out1++=*cin_it++;   //not forget the ++
		}
		else
		{
			*out2++=*cin_it++;
		}

	}
		of1.close(); 
		of2.close(); 
		cout << endl; 



	system("pause"); 
	return  0; 
}



