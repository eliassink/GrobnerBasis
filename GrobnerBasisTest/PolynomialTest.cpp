#include "pch.h"
#include "../GrobnerBasisLib/Polynomial.h"
#include "../GrobnerBasisLib/LexTermOrder.h"
#include "../GrobnerBasisLib/DegLexTermOrder.h"
#include "../GrobnerBasisLib/Rational.h"

template <typename _T>
class PolynomialTest : public testing::Test
{
protected:
	using T = _T;
	using P = Polynomial<T>;
	P x{ PowerProduct(0)};
	P y{ PowerProduct(1) };
	LexTermOrder lex;
	DegLexTermOrder deglex;
};

using CoefficientTypes = testing::Types<double,Rational<>>;
TYPED_TEST_CASE(PolynomialTest, CoefficientTypes);

TYPED_TEST(PolynomialTest, AdditionTest)
{
	EXPECT_EQ(x + x, 2 * x);
	EXPECT_EQ(x + x + y + y + y, 2 * x + 3 * y);
}

TYPED_TEST(PolynomialTest, NegationTest)
{
	EXPECT_EQ(-x, -1 * x);
	EXPECT_EQ(-(x+y), -x+(-y));
}

TYPED_TEST(PolynomialTest, SubtractionTest)
{
	EXPECT_EQ(3 * x - 2 * x, x);
	EXPECT_EQ(x - x, 0);
}

TYPED_TEST(PolynomialTest, MultiplicationTest)
{
	EXPECT_EQ(x * x, P(PowerProduct(0).pow(2)));
	EXPECT_EQ((2 * x) * (3 * y), 6 * P(PowerProduct(0)*PowerProduct(1)));
	EXPECT_EQ((x + y) * (x - y), x * x - y * y);
}

TYPED_TEST(PolynomialTest, PowerTest)
{
	EXPECT_EQ(x.pow(5), P(PowerProduct(0).pow(5)));
	EXPECT_EQ((x + y).pow(2), x.pow(2) + 2 * x * y + y.pow(2));
	EXPECT_EQ((x * y).pow(7), x.pow(7) * y.pow(7));
}

TYPED_TEST(PolynomialTest, DivisibleByTest)
{
	EXPECT_TRUE((x.pow(2) * y.pow(3)).isDivisibleBy(x));
	EXPECT_FALSE((x.pow(2) * y.pow(3)).isDivisibleBy(x.pow(3)));
	EXPECT_TRUE((2 * x.pow(2) * y - 3 * x * y.pow(2)).isDivisibleBy(y));
	EXPECT_FALSE((2 * x.pow(2) * y - 3 * x * y.pow(2) + x).isDivisibleBy(y));
	EXPECT_FALSE((x.pow(2) - y.pow(2)).isDivisibleBy( x - y)) << "Only divide by monomials";
	EXPECT_TRUE(x.isDivisibleBy(1));
	EXPECT_FALSE(x.isDivisibleBy(0));
}

TYPED_TEST(PolynomialTest, DivisionTest)
{
	EXPECT_EQ((x.pow(2) * y.pow(3)) / x, x * y.pow(3));
	EXPECT_THROW((x.pow(2) * y.pow(3)) / x.pow(3), std::exception);
	EXPECT_EQ((2 * x.pow(2) * y - 3 * x * y.pow(2))/y, 2 * x.pow(2) - 3 * x * y);
	EXPECT_EQ(x / 1, x);
}

TYPED_TEST(PolynomialTest, LeadingPowerProductTest)
{
	EXPECT_EQ(x.leadingPower(lex), PowerProduct(0));
	EXPECT_EQ(
		(2 * x.pow(2) * y + x * y.pow(3)).leadingPower(lex), 
		PowerProduct(0).pow(2)*PowerProduct(1)
	);
	EXPECT_EQ(
		(2 * x.pow(2) * y + x * y.pow(3)).leadingPower(deglex), //dynamic term order
		PowerProduct(0) * PowerProduct(1).pow(3)
	); 
	EXPECT_THROW(P(0).leadingPower(lex),std::exception);
}

TYPED_TEST(PolynomialTest, LeadingCoefTest)
{
	EXPECT_EQ(x.leadingCoef(lex), 1);
	EXPECT_EQ((2 * x.pow(2) * y + x * y.pow(3)).leadingCoef(lex), 2);
	EXPECT_EQ((2 * x.pow(2) * y + x * y.pow(3)).leadingCoef(deglex), 1);
	EXPECT_EQ(P(0).leadingCoef(lex), 0);
}

TYPED_TEST(PolynomialTest, LeadingTermTest)
{
	EXPECT_EQ((x + y).leadingTerm(lex), x);
	EXPECT_EQ((x.pow(3) * y.pow(2) - 2 * x.pow(3) * y.pow(3) + 5 * x * y).leadingTerm(lex),
		-2 * x.pow(3) * y.pow(3));
	EXPECT_EQ((x.pow(3) * y.pow(2) - 2 * x * y.pow(5) + 5 * x * y).leadingTerm(deglex),
		-2 * x * y.pow(5)
	);
	EXPECT_EQ(P(1).leadingTerm(lex), 1);
	EXPECT_EQ(P(0).leadingTerm(lex), 0);
}