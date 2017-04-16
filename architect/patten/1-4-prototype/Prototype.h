#ifndef _PROTOTYPE_H_
#define _PROTOTYPE_H_

class Prototype
{
public:
	virtual ~Prototype();
	virtual Prototype* Clone() const = 0;
protected:
	Prototype();
};

class Prototype1:public Prototype
{
public:
	Prototype1();
	Prototype1(const Prototype1& cp);
	~Prototype1();
	Prototype* Clone() const;
};

class Prototype2:public Prototype
{
public:
	Prototype2();
	Prototype2(const Prototype2& cp);
	~Prototype2();
	Prototype* Clone() const;
};

#endif //~_PROTOTYPE_H_
