#include "stdafx.h"
#include "Item.h"
#include "Basket.h"
#include "Sales_item.h"
int _tmain( int argc, _TCHAR* argv[]) 
{ 
	Basket basket; 
	Sales_item item1 ( Bulk_item(  "0-0001-0001-1" , 2, 2,  0.5 )); 
	Sales_item item2 ( Bulk_item(  "0-0001-0001-1" , 2, 2,  0.5 )); 
	Sales_item item3 ( Bulk_item(  "0-0001-0001-1" , 2, 2,  0.5 )); //3本会打折   2*3*0.5
    Sales_item item4 ( Ltd_item (  "0-0001-0001-2" , 90, 2,  0.5 )); //1本会打折    9*1*0.5
	basket.add_item(item1); 
	basket.add_item(item2); 
	basket.add_item(item3); 
    basket.add_item(item4); 
	cout << basket.total() << endl; 
	Sales_item   itemP(item4);
    cout<<itemP->net_price (1)<<"******";
	system("pause"); 
	return  0; 
}



