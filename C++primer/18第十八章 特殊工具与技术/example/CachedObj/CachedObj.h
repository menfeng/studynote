/* memory allocation class: Pre-allocates objects and
* maintains a freelist of objects that are unused
* When an object is freed, it is put back on the freelist
* The memory is only returned when the program exits
*/
template <class T> class CachedObj {
public:
	void *operator new(std::size_t);
	void operator delete(void *, std::size_t);
	virtual ~CachedObj() { }
protected:
	T *next;
private:
	static void add_to_freelist(T*);
	static std::allocator<T> alloc_mem;
	static T *freeStore;
	static const std::size_t chunk;
};

template <class T> allocator< T > CachedObj< T >::alloc_mem;
template <class T> T*             CachedObj< T >::freeStore = 0;
template <class T> const size_t   CachedObj< T >::chunk = 24;

template <class T>
void *CachedObj<T>::operator new(size_t sz)
{
	// new should only be asked to build a T, not an object
	// derived from T; check that right size is requested
	if (sz != sizeof(T))
		throw std::runtime_error
		("CachedObj: wrong size object in operator new");
	if (!freeStore) {
		// the list is empty: grab a new chunk of memory
		// allocate allocates chunk number of objects of type T
		T * array = alloc_mem.allocate(chunk);

		// now set the next pointers in each object in the allocated memory
		for (size_t i = 0; i != chunk; ++i)
			add_to_freelist(&array[i]);
	}
	T *p = freeStore;
	freeStore = freeStore->CachedObj<T>::next;
	return p;   // constructor of T will construct the T part of the object
}

template <class T>
void CachedObj<T>::operator delete(void *p, size_t)
{
	if (p != 0)
		// put the "deleted" object back at head of freelist
		add_to_freelist(static_cast<T*>(p));
}

// puts object at head of the freelist
template <class T>
void CachedObj<T>::add_to_freelist(T *p)
{
	p->CachedObj<T>::next = freeStore;//注意此处使用了作用域，防止与子类名字冲突
	freeStore = p;
}

