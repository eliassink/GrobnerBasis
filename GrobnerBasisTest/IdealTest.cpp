#include "pch.h"
#include "../GrobnerBasisLib/LexTermOrder.h"
#include "../GrobnerBasisLib/DegLexTermOrder.h"
#include "../GrobnerBasisLib/Rational.h"
#include "../GrobnerBasisLib/Ideal.h"


class IdealTest : public testing::Test
{
protected:
	using Q = Rational<>;
	using P = Polynomial<Q>;
	using I = Ideal<Q>;
	P x{ PowerProduct(1)};
	P y{ PowerProduct(0)};
	I i{ {x * y - x,-y + x.pow(2)}};  //init list, default term order

	P basis[3]{ y.pow(2) + y * x + x.pow(2),y + x,y };
	I j{ basis,basis + 3,  std::make_unique<LexTermOrder>() }; //iterator, specified term order
};

TEST_F(IdealTest, EqualsTest)
{
	EXPECT_TRUE(i.equals(I({ x * y - x, -y + x.pow(2), x.pow(3) - x }))); //redundant generator
	EXPECT_TRUE(j.equals(I({ y, x }))); //obvious simplification
	EXPECT_FALSE(j.equals(I({ x + 1, y })));
}

TEST_F(IdealTest, MemberTest)
{
	EXPECT_TRUE(j.isMember(3 * x.pow(5) - 2 * y * x));
	EXPECT_FALSE(j.isMember(x.pow(2) + 3));
	EXPECT_TRUE(i.isMember((x * y - x) * y * x - (-y + x.pow(2)) * x.pow(2)));
}

TEST_F(IdealTest, ReduceTest)
{
	EXPECT_EQ(j.reduce(x.pow(2) + y - 3), -3);
}

TEST_F(IdealTest, ContainsTest)
{
	EXPECT_TRUE(I({ x,y }).contains(I({ x })));
	EXPECT_FALSE(I({ x }).contains(I({ x,y })));
}

TEST_F(IdealTest, SetTermOrderTest)
{
	I k{ { x.pow(2) - y },std::make_unique<LexTermOrder>() };

	EXPECT_EQ(k.reduce(y), x.pow(2));

	k.setTermOrder(std::make_unique<DegLexTermOrder>());

	EXPECT_EQ(k.reduce(y), y);

	EXPECT_THROW(k.setTermOrder(nullptr), std::exception);
}