#include "pch.h"
#include "../GrobnerBasisLib/PowerProduct.h"
#include <exception>

class PowerProductTest : public testing::Test {
protected:
	PowerProduct powerProduct(std::initializer_list<int> degrees)
	{
		PowerProduct pp;
		int i = 0;
		for (int d : degrees)
			pp *= PowerProduct(i++).pow(d);
		return pp;
	}
};


TEST_F(PowerProductTest, EqualityTest) {
	EXPECT_EQ(powerProduct({ 1, 2, 3 }), powerProduct({ 1, 2, 3 }));
	EXPECT_NE(powerProduct({ 1,2,3 }), powerProduct({ 2,3,5 }));
	EXPECT_EQ(powerProduct({ 1,2,0 }), powerProduct({ 1,2 }));
}

TEST_F(PowerProductTest, MultiplicationTest) {
	
	EXPECT_EQ(powerProduct({ 1,2,3 }) * powerProduct({ 2,3,5 }), powerProduct({ 3, 5, 8 }));
	EXPECT_EQ(powerProduct({ 1,2,3 }) * powerProduct({ 2,3 }), powerProduct({ 3, 5, 3 }));
	EXPECT_EQ(powerProduct({ 2,3 }) * powerProduct({ 1,2,3 }), powerProduct({ 3, 5, 3 }));
	EXPECT_EQ(powerProduct({}) * powerProduct({ 1,2,3 }), powerProduct({ 1,2,3 }));
}

TEST_F(PowerProductTest, IsDivisibleByTest) {
	EXPECT_TRUE(powerProduct({ 1,2,3,4 }).isDivisibleBy(powerProduct({ 0,1,2 })));
	EXPECT_FALSE(powerProduct({ 1,0,3 }).isDivisibleBy(powerProduct({ 0,1 })));
	EXPECT_TRUE(powerProduct({ 1,2 }).isDivisibleBy(powerProduct({ 1,2 })));
}

TEST_F(PowerProductTest, DivisionTest) {
	EXPECT_EQ(powerProduct({ 1,2,3,4 }) / powerProduct({ 0,1,2 }), powerProduct({1,1,1,4}));
	EXPECT_THROW(powerProduct({ 1,0,3 }) / powerProduct({ 0,1 }), std::exception);
}

TEST_F(PowerProductTest, PowerTest)
{
	EXPECT_EQ(powerProduct({}).pow(5), powerProduct({}));
	EXPECT_EQ(powerProduct({ 1,2,3 }).pow(3), powerProduct({ 3,6,9 }));
	EXPECT_THROW(powerProduct({ 1 }).pow(-1), std::exception);
}

TEST_F(PowerProductTest, LCMTest)
{
	EXPECT_EQ(powerProduct({ 1,2 }).lcm(powerProduct({ 2, 1 })), powerProduct({2,2}));
	EXPECT_EQ(powerProduct({}).lcm(powerProduct({ 1,2,3 })), powerProduct({ 1,2,3 }));
}