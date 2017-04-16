#include "Builder.h"
#include "Director.h"

/*
 * Separate the construction of a complex object from its representation so that the same construction process can create different representations.
 */

int main(int argc,char* argv[])
{
	Director* director = new Director();

	Builder* builder1_1=new Builder1();
	director->Construct1(builder1_1);

	Builder* builder1_2=new Builder1();
	director->ConstructComplex1(builder1_2);

	Builder* builder2_1=new Builder2();
	director->Construct1(builder2_1);
	Builder* builder2_2=new Builder2();
	director->ConstructComplex1(builder2_2);
	return 0;
}
