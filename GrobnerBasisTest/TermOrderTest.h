#pragma once
#include "../GrobnerBasisLib/PowerProduct.h"

class TermOrderTest
{
protected:
	//construct a power product with the given degrees
	PowerProduct powerProduct(std::initializer_list<int> degrees)
	{
		PowerProduct pp;
		int i = 0;
		for (int d : degrees)
			pp *= PowerProduct(i++).pow(d);
		return pp;
	}
};

