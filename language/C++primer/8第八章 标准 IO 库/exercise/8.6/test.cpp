/*
* A program to transform words.
* Takes two arguments: The first is name of the word transformation file
*                      The second is name of the input to transform
*/
#include "stdafx.h"

istream  &  f(  istream  &  is  )   
{   
	//int ival;     
	string ival ;   

	while (  is  >>  ival )      
	{         
		if (is .bad())   
		{
			throw  runtime_error ( "IO stream corrupted"); 
		}
			  
		if  (  is . fail() )                   
		{   
			cerr  <<  " bad data, try again." ;   
			is .clear ();                                     
			continue;   
		}     

		cout  <<  ival  <<  endl ;   
	}     
	is .clear ();  
	return  is ;   
}   
int _tmain (int argc ,  _TCHAR *  argv [])   
{   
	string fName ;   
	cout  <<  " Input name of file( ctrl + z to end ):\n" ;     
	cin >>  fName ;   
	ifstream readfile;   
	readfile. open ( fName .c_str ());  // open the file   
	if  (! readfile)   
	{   
		cerr  <<  " error: cannot open the input file:"  << fName  <<  endl ;   
		return  -1;    
	}     
	f( readfile);   
	system ( "pause");   
	return  0;   
}   


