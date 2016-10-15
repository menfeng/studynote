#include "stdafx.h"
#include "Queue.h"

// program takes single argument specifying the file to query
int main(int argc, char **argv) 
{
	int a[]={1,2,3,4};
	Queue<int>  intQ(a,a+4);
	for(int i=0;i<50;i++)
	intQ.push(8);
	Queue<int> intQ1(intQ);
	intQ1=intQ;
	intQ1.pop ();
	cout<<intQ1.front ()<<endl;
	cout<<intQ1;

	system("pause");
	return 0;
}



