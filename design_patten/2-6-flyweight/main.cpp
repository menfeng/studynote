#include "Flyweight.h"
#include "FlyweightFactory.h"
#include <iostream>
using namespace std;

/*
 *Use sharing to support large numbers of fine-grained objects efficiently
 */

int main(int argc,char* argv[])
{
	FlyweightFactory* fc = new FlyweightFactory();
	Flyweight* fw1 = fc->GetFlyweight("hello");
	Flyweight* fw2 = fc->GetFlyweight("world!");
	Flyweight* fw3 = fc->GetFlyweight("hello");
	return 0;
}
