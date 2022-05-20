#include "pch.h"
#include "../GrobnerBasisLib/Rational.h"
#include <sstream>

class RationalTest : public testing::Test
{
protected:
	using R = Rational<int>;
};

TEST_F(RationalTest, EqualityTest)
{
	EXPECT_EQ(R(3), R(3));
	EXPECT_EQ(R(3, 4), R(3, 4));
	EXPECT_EQ(R(6, 4), R(3, 2));
}

TEST_F(RationalTest, AdditionTest)
{
	EXPECT_EQ(R(3) + R(2), R(5));
	EXPECT_EQ(R(2, 3) + R(3, 4), R(17, 12));
}

TEST_F(RationalTest, NegationTest)
{
	EXPECT_EQ(-R(3, 2), R(-3, 2));
}

TEST_F(RationalTest, SubtractionTest)
{
	EXPECT_EQ(R(2, 3) - R(3, 4), R(-1, 12));
}

TEST_F(RationalTest, MultiplicationTest)
{
	EXPECT_EQ(R(2, 3) * R(4, 7), R(8, 21));
	EXPECT_EQ(R(-2, 5) * R(3, 4), R(-3, 10));
}

TEST_F(RationalTest, DivisionTest)
{
	EXPECT_EQ(R(2, 3) / R(7, 4), R(8, 21));
	EXPECT_EQ(R(-2, 5) / R(4, 3), R(-3, 10));
	EXPECT_THROW(R(1, 2) / R(0), std::exception);
}

TEST_F(RationalTest, CompareTest)
{
	EXPECT_LT(R(-1), R(1));
	EXPECT_LT(R(1, 4), R(1, 3));
}

TEST_F(RationalTest, InsertionTest)
{
	std::stringstream ss;
	ss << R(1) << R(-1) << R(1, 2) << R(-2, 3);
	EXPECT_EQ(ss.str(), "1-11/2-2/3");
}

TEST_F(RationalTest, ExtractionTest)
{
	R input;
	std::stringstream ss("2 1/2 -3/2");
	ss >> input;
	EXPECT_EQ(input, R(2));
	ss >> input;
	EXPECT_EQ(input, R(1, 2));
	ss >> input;
	EXPECT_EQ(input, R(-3, 2));
}