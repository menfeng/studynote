/* generic handle class: Provides pointerlike behavior. Although access through
* an unbound Handle is checked and throws a runtime_error exception.
* The object to which the Handle points is deleted when the last Handle goes away.
* Users should allocate new objects of type T and bind them to a Handle.
* Once an object is bound to a Handle,, the user must not delete that object.
(1)T不能是一个抽象类，否则当用一个抽象类的指针初始化此类后，当进行解引用时就出
   问题了，因为根本没有基础对象
(2)不要自己删除基础对象
*/

template <class T> 
class Handle {
public:
	// unbound handle
	Handle(T *p = 0): ptr(p), use(new size_t(1)) { }
	// overloaded operators to support pointer behavior
	T& operator*();
	T* operator->();
	const T& operator*() const;
	const T* operator->() const;
	// copy control: normal pointer behavior, but last Handle deletes the object
	Handle(const Handle& h): ptr(h.ptr), use(h.use){ ++*use; }
	Handle& operator=(const Handle&);
	~Handle() { rem_ref(); }
private:
	T* ptr;          // shared object
	size_t *use;     // count of how many Handle spointto *ptr
	void rem_ref()
	{ if (--*use == 0) { delete ptr; delete use; } }
};

template <class T>
inline Handle<T>& Handle<T>::operator=(const Handle &rhs)
{
	++*rhs.use;      // protect against self-assignment
	rem_ref();       // decrement use count and delete pointers if needed
	ptr = rhs.ptr;
	use = rhs.use;
	return *this;
}

template <class T> inline 
T& Handle<T>::operator*()
{
	if (ptr) return *ptr;
	throw std::runtime_error("dereference of unbound Handle");
}

template <class T> inline 
const T& Handle<T>::operator*() const
{
	if (ptr) return *ptr;
	throw std::runtime_error("dereference of unbound Handle");
}


template <class T> inline 
T* Handle<T>::operator->()
{
	if (ptr) return ptr;
	throw std::runtime_error("access through unbound Handle");
}

template <class T> inline 
const T* Handle<T>::operator->() const
{
	if (ptr) return ptr;
	throw std::runtime_error("access through unbound Handle");
}

