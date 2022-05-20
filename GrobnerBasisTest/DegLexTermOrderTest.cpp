#include "pch.h"
#include "../GrobnerBasisLib/DegLexTermOrder.h"

//test the degree lexicographical term order
class DegLexTermOrderTest : public testing::Test
{
protected:
	DegLexTermOrder deglex;
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

//Test degree lexicographical comparison
TEST_F(DegLexTermOrderTest, CompareTest)
{
	ASSERT_FALSE(deglex(powerProduct({}), powerProduct({})));
	ASSERT_FALSE(deglex(powerProduct({1,2}), powerProduct({1,2})));
	ASSERT_TRUE(deglex(powerProduct({}), powerProduct({ 1 })));
	ASSERT_TRUE(deglex(powerProduct({ 1 }), powerProduct({ 0,2 })));
	ASSERT_TRUE(deglex(powerProduct({ 1,2 }), powerProduct({ 2,1 })));
}