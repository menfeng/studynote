#ifndef    ITEM_H 
#define    ITEM_H 
#include "stdafx.h"
/**************************Item_base***************************/
class  Item_base 
{ 
public: 
	Item_base(  const string &book =  "" ,  double sales_price = 0.0): 
	isbn( book ), 
	price( sales_price )        
	{  
	} 
	string book( )  const   
	{ 
		return  isbn; 
	} 

	virtual   double  net_price( size_t n )  const 
	{ 
		return  price * n; 
	} 
	virtual  ~Item_base()   
	{      
	} 

	virtual  Item_base* clone() const 
	{ 
		return   new  Item_base( * this  ); 
	} 
private : 
	string isbn; 
protected: 
	double  price; 
}; 

/**************************Disc_item***************************/
class  Disc_item:  public Item_base 
{ 
public: 
	Disc_item (const string& book = "",double sales_price = 0.0,size_t qty = 0,double  disc = 0.0): 
	Item_base ( book, sales_price ), 
	quantity ( qty ), 
	discount ( disc )   
	{  
	} 
	virtual   double  net_price( size_t )  const = 0; 
	std::pair<size_t ,  double  > discount_policy() const 
	{  return  std::make_pair( quantity, discount );    } 
	~Disc_item() 
	{  
	} 
protected: 
	size_t quantity; 
	double  discount; 
}; 

/**************************Bulk_item***************************/

class  Bulk_item : public Disc_item 
{ 
public: 
	Bulk_item (const string &book ="",double sales_price = 0.0,size_t qty = 0,double disc = 0.0 ): 
	Disc_item( book, sales_price, qty, disc ) 
	{  
	} 
	double  net_price( size_t cnt ) const 
	{ 
		if  ( cnt > quantity ) 
			return  cnt * ( 1- discount ) * price; 
		else 
			return  cnt * price; 
	} 

	virtual  Bulk_item* clone() const 
	{ 
		return   new  Bulk_item( * this  ); 
	} 
}; 

class  Ltd_item :  public Disc_item 
{ 
public: 
	Ltd_item( const string& book ="",double sales_price = 0.0,size_t qty = 0,double disc_rate =0.0): 
	Disc_item( book, sales_price, qty, disc_rate ) 
	{    
	} 
	double  net_price( size_t cnt ) const 
	{ 
		if  ( cnt <= quantity ) 
			return     cnt * ( 1- discount ) * price; 
		else 
			return     cnt* price - quantity * discount * price; 
	} 

	virtual  Ltd_item* clone()  const 
	{ 
		return   new  Ltd_item( * this  ); 
	} 

}; 
#endif  









