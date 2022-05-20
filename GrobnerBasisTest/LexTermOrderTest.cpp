#include "pch.h"
#include "../GrobnerBasisLib/PowerProduct.h"
#include "../GrobnerBasisLib/LexTermOrder.h"

class LexTermOrderTest : public testing::Test
{
protected:
	LexTermOrder lex{};
	PowerProduct powerProduct(std::initializer_list<int> degrees)
	{
		PowerProduct pp;
		int i = 0;
		for (int d : degrees)
			pp *= PowerProduct(i++).pow(d);
		return pp;
	}
	
};

TEST_F(LexTermOrderTest, CompareTest)
{
	EXPECT_FALSE(lex(powerProduct({}), powerProduct({})));
	EXPECT_FALSE(lex(powerProduct({ 1, 2, 3 }), powerProduct({ 1, 2, 3 })));

	EXPECT_FALSE(lex(powerProduct({ 1, 2, 3 }), powerProduct({})));
	EXPECT_FALSE(lex(powerProduct({ 2,1 }), powerProduct({ 1, 2 })));
	EXPECT_FALSE(lex(powerProduct({ 2, 2 }), powerProduct({ 2, 1, 1 })));
	EXPECT_FALSE(lex(powerProduct({ 2, 1 }), powerProduct({ 2 })));

	EXPECT_TRUE(lex(powerProduct({ 2, 3, 2 }), powerProduct({ 2, 4 })));
	EXPECT_TRUE(lex(powerProduct({}), powerProduct({ 1 })));
}
