#include "pch.h"
#include "../GrobnerBasisLib/RationalParser.h"
#include "../GrobnerBasisLib/LexTermOrder.h"

class RationalParserTest : public testing::Test
{
protected:
	RationalParser parser{ "x","y" };
	Polynomial<Rational<>> x{ PowerProduct(0) };
	Polynomial<Rational<>> y{ PowerProduct(1) };
};

TEST_F(RationalParserTest, ParseTest)
{
	EXPECT_EQ(parser.parse("1/2*x^2+y*x-6"), Rational<>(1, 2) * x.pow(2) + y * x - 6);
	EXPECT_EQ(parser.parse("-x^3*y^2 - 2*y"), -x.pow(3) * y.pow(2) - 2 * y);
	EXPECT_EQ(parser.parse("x"), x);
	EXPECT_EQ(parser.parse("-y"), -y);
	EXPECT_EQ(parser.parse("0"), 0);
	EXPECT_EQ(parser.parse("1/2"), Rational<>(1, 2));
	EXPECT_EQ(parser.parse("-1/2"), -Rational<>(1, 2));
}