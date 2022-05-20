#include "pch.h"
#include "../GrobnerBasisLib/LexTermOrder.h"
#include "../GrobnerBasisLib/Rational.h"
#include "../GrobnerBasisLib/Ideal.h"

class IdealTest : public testing::Test
{
protected:
	using Q = Rational<int>;
	using P = Polynomial<Q, LexTermOrder>;
	using I = Ideal<Q, LexTermOrder>;
	P x{ PowerProduct(1)};
	P y{ PowerProduct(0)};
	I i{ {x * y - x,-y + x.pow(2)} };  //init list

	P basis[3]{ y.pow(2) + y * x + x.pow(2),y + x,y };
	I j{ basis,basis + 3 }; //iterator
};

TEST_F(IdealTest, EqualityTest)
{
	EXPECT_EQ(i, I({ x * y - x, -y + x.pow(2), x.pow(3) - x })); //redundant generator
	EXPECT_EQ(j, I({ y, x })); //obvious simplification
	EXPECT_NE(j, I({ x + 1, y}));
}

TEST_F(IdealTest, ContainsTest)
{
	EXPECT_TRUE(j.contains(3 * x.pow(5) - 2 * y * x));
	EXPECT_FALSE(j.contains(x.pow(2) + 3));
	EXPECT_TRUE(i.contains((x * y - x) * y * x - (-y + x.pow(2)) * x.pow(2)));
}

TEST_F(IdealTest, ReduceTest)
{
	EXPECT_EQ(j.reduce(x.pow(2) + y - 3), -3);
}