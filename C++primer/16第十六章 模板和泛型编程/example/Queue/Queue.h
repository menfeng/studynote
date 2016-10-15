#ifndef MY_QEUEN
#define MY_QEUEN

template <class Type> class Queue;
template <class T>    std::ostream& operator<<(std::ostream&, const Queue<T>&);

template <class Type> class QueueItem 
{
	friend class Queue<Type>;
	friend std::ostream&  operator<< <Type> (std::ostream&, const Queue<Type>&);
	QueueItem(const Type &t): item(t), next(0) { }
	Type item;          
	QueueItem *next;
};

template <class Type> class Queue 
{
	friend std::ostream& operator<< <Type> (std::ostream&, const Queue<Type>&);
public:
	Queue(): head(0), tail(0) { }
	template <class It> Queue(It beg, It end):head(0), tail(0) { copy_elems(beg, end); }
	Queue(const Queue &Q): head(0), tail(0){ copy_elems(Q); }
	Queue& operator=(const Queue&); // left as exercise for the reader
	~Queue() { destroy(); }
	template <class Iter> void assign(Iter, Iter);
	Type& front()             { return head->item; }
	const Type &front() const { return head->item; }
	void push(const Type &);
	void pop();             
	bool empty() const {return head == 0;}
private:
	QueueItem<Type> *head;   
	QueueItem<Type> *tail;   
	void destroy();                
	void copy_elems(const Queue&);
	template <class Iter> void copy_elems(Iter, Iter);
};
// Inclusion Compilation Model: include member function definitions as well
#include "Queue.cpp"
#endif






