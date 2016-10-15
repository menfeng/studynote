#ifndef _LEAF_H_
#define _LEAF_H_

#include <string>
#include "Component.h"

using namespace std;

class Leaf:public Component {
public:
	Leaf(const char* s);
	~Leaf();
	void Operation();

private:
	string _name;
};

#endif //~_LEAF_H_
