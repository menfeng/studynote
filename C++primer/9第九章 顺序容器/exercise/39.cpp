// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define offset(struct_name, element_name);

int _tmain(int argc, _TCHAR* argv[])
{
	
	string separators(" :\t," ); 
	// find how many words
	string line1 =  "We were her pride of 10 she named us:" ; 
	string line2 =  "Benjamin, Phoenix, the Prodigal" ; 
	string line3 =  "and perspicacious pacific Suzanne" ; 
	string sentence = line1 + ' '  + line2 +  ' '  + line3; 
	string word; 
	string::size_type maxLen, minLen, wordLen; 
	vector<string> longestWords, shortestWords; 

	cout <<  "\n The sentence is :\n"  << sentence << endl; 
	string::size_type startPos = 0, endPos = 0; 
	size_t cnt = 0; 
	while ( ( startPos = sentence.find_first_not_of( separators, endPos )) != string::npos ) 
	{ 
		++cnt; 
		endPos = sentence.find_first_of( separators, startPos ); 
		if  ( endPos == string::npos ) 
			wordLen = sentence.size() - startPos; 
		else 
			wordLen = endPos - startPos; 

		word.assign( sentence.begin() + startPos,sentence.begin() + startPos + wordLen ); 
		if  ( cnt == 1 ) //如果是第一个单词
		{ 
			maxLen = minLen = wordLen; 
			longestWords.push_back( word ); 
			shortestWords.push_back( word ); 
		} 
		else 
		{ 
			if  ( wordLen > maxLen ) 
			{ 
				maxLen = wordLen; 
				longestWords.clear(); 
				longestWords.push_back( word ); 
			} 
			else   if  ( wordLen == maxLen ) 
			{ 
				longestWords.push_back( word ); 
			} 

			if  ( wordLen < minLen ) 
			{ 
				minLen = wordLen; 
				shortestWords.clear(); 
				shortestWords.push_back( word ); 
			} 
			else   if  ( wordLen == minLen ) 
			{ 
				shortestWords.push_back(word); 
			} 
		} 
	} 

	// cout number of words 
	cout <<  "\n\t There are "  << cnt << " words in the sentence." << endl; 
		vector<string>::iterator iter; 
	// out the longest word 
	cout <<  "\n\t longest word :"  << endl; 
	iter = longestWords.begin(); 
	while ( iter != longestWords.end() ) 
	{ 
		cout << *iter++ << endl; 
	} 

	// out the shortest word 
	cout <<  "\n\t shortest word :"  << endl; 
	iter = shortestWords.begin(); 
	while ( iter != shortestWords.end() ) 
	{ 
		cout << *iter++ << endl; 
	}
	system("pause");
	return 0;
}

