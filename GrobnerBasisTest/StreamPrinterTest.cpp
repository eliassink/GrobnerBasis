#include "pch.h"
#include "../GrobnerBasisLib/StreamPrinter.h"
#include "../GrobnerBasisLib/Rational.h"
#include "../GrobnerBasisLib/Ideal.h"
#include "../GrobnerBasisLib/LexTermOrder.h"
#include "../GrobnerBasisLib/DegLexTermOrder.h"

class StreamPrinterTest : public testing::Test
{
protected:
	using Q = Rational<>;
	using P = Polynomial<Q>;
	using I = Ideal<Q>;
	StreamPrinter<Q> defaultPrinter;
	StreamPrinter<Q> customPrinter{"x","y"};
	DegLexTermOrder deglex;
	P x{ PowerProduct(0)};
	P y{ PowerProduct(1) };
	P f{ 1 + x - y.pow(2) };
	P g{ Q(1,2) - Q(3,2) * y - 5 * x.pow(2)};
	P h{ -3 * x * y.pow(2) - 2 };
	I i{ x };
	I j{ -x,y + 1 };
};

TEST_F(StreamPrinterTest, DefaultTest)
{
	EXPECT_EQ(P().toString(defaultPrinter), "0");
	EXPECT_EQ(x.toString(defaultPrinter), "x1");
	EXPECT_EQ((-y).toString(defaultPrinter), "-x2");
	EXPECT_EQ(f.toString(defaultPrinter), "x1 - x2^2 + 1");
	EXPECT_EQ(g.toString(defaultPrinter), "-5*x1^2 - 3/2*x2 + 1/2");
	EXPECT_EQ(h.toString(defaultPrinter), "-3*x1*x2^2 - 2");
	EXPECT_EQ(i.toString(defaultPrinter), "( x1 )");
	EXPECT_EQ(j.toString(defaultPrinter), "( x1 , x2 + 1 )");

	//dynamic term order
	EXPECT_EQ(f.toString(defaultPrinter,deglex), "-x2^2 + x1 + 1");
	EXPECT_EQ(g.toString(defaultPrinter,deglex), "-5*x1^2 - 3/2*x2 + 1/2");
}

TEST_F(StreamPrinterTest, CustomTest)
{
	EXPECT_EQ(P().toString(customPrinter), "0");
	EXPECT_EQ(x.toString(customPrinter), "x");
	EXPECT_EQ((-y).toString(customPrinter), "-y");
	EXPECT_EQ(f.toString(customPrinter), "x - y^2 + 1");
	EXPECT_EQ(g.toString(customPrinter), "-5*x^2 - 3/2*y + 1/2");
	EXPECT_EQ(h.toString(customPrinter), "-3*x*y^2 - 2");
	EXPECT_EQ(i.toString(customPrinter), "( x )");
	EXPECT_EQ(j.toString(customPrinter), "( x , y + 1 )");
}
