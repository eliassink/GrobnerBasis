#include "pch.h"
#include "TermOrderTest.h"
#include "../GrobnerBasisLib/DegRevLexTermOrder.h"

class DegRevLexTermOrderTest : public testing::Test, public TermOrderTest
{
protected:
	DegRevLexTermOrder drlex;
};

TEST_F(DegRevLexTermOrderTest, CompareTest)
{
	EXPECT_TRUE(drlex(powerProduct({ 0,1,1 }), powerProduct({ 2,1,0 })));
	EXPECT_TRUE(drlex(powerProduct({ 1,0,1 }), powerProduct({ 2,0,0 })));
	EXPECT_TRUE(drlex(powerProduct({ 1,0,2 }), powerProduct({ 0,2,1 })));
}