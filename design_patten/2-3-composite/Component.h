#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class Component {
public:
	Component();
	virtual ~Component();
	virtual void Operation() = 0;
	virtual void Add(Component* com);
	virtual void Remove(Component* com);
	virtual Component* GetChild(int );
};

#endif //~_COMPONENT_H_
