#ifndef  __BASKET_H_ 
#define    __BASKET_H_ 
#include "Sales_item.h" 

inline  bool  compare(const Sales_item &lhs, const Sales_item &rhs ) 
{ 
	return  lhs->book() < rhs->book(); 
} 

class  Basket 
{ 
	typedef  bool  (*Comp) (const Sales_item&,  const Sales_item&); 
public: 
	typedef std::multiset<Sales_item, Comp> set_type; 
	typedef set_type::size_type             size_type; 
	typedef set_type::const_iterator        const_iter; 

	Basket(): items( compare )
	{
	} 

	void  add_item(  const Sales_item &item ) 
	{ 
		items.insert( item ); 
	} 

	size_type size( const Sales_item &i )  const 
	{ 
		return  items.count(i); 
	} 

	double  total()  const; 
private : 
	std::multiset<Sales_item, Comp> items; 

}; 

double  Basket::total() const 
{ 
	double  sum = 0.0; 
	for ( const_iter iter = items.begin(); iter != items.end(); iter = items.upper_bound(*iter)) 
	{ 
		sum += (*iter)->net_price( items.count( *iter )); 
	} 
	return  sum; 
} 

#endif 