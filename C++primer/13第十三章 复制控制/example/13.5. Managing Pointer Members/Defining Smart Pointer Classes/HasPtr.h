#include "stdafx.h"
/* smart pointer class: takes ownership of the dynamically allocated
*          object to which it is bound
* User code must dynamically allocate an object to initialize a HasPtr
* and must not delete that object; the HasPtr class will delete it
*/
class HasPtr {
public:
	// HasPtr owns the pointer; pmust have been dynamically allocated
	HasPtr(int *p, int i): ptr(new U_Ptr(p)), val(i) { }
	// copy members and increment the use count
	HasPtr(const HasPtr &orig):
	ptr(orig.ptr), val(orig.val) { ++ptr->use; }
	HasPtr& operator=(const HasPtr&);
	// if use count goes to zero, delete the U_Ptr object
	~HasPtr() { if (--ptr->use == 0) delete ptr; }

	// copy control and constructors as before
	// accessors must change to fetch value from U_Ptr object
	int *get_ptr() const { return ptr->ip; }
	int get_int() const { return val; }

	// change the appropriate data member
	void set_ptr(int *p) { ptr->ip = p; }
	void set_int(int i) { val = i; }

	// return or change the value pointed to, so ok for const objects
	// Note: *ptr->ip is equivalent to *(ptr->ip)
	int get_ptr_val() const { return *ptr->ip; }
	void set_ptr_val(int i) { *ptr->ip = i; }


private:
	U_Ptr *ptr;        // points to use-counted U_Ptr class
	int val;
};

HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
	++rhs.ptr->use;     // increment use count on rhs first
	if (--ptr->use == 0)
		delete ptr;    // if use count goes to 0 on this object, delete it
	ptr = rhs.ptr;      // copy the U_Ptr object
	val = rhs.val;      // copy the int member
	return *this;
}



// private class for use by HasPtr only
class U_Ptr {
	friend class HasPtr;
	int *ip;
	size_t use;
	U_Ptr(int *p): ip(p), use(1) { }
	~U_Ptr() { delete ip; }
};

