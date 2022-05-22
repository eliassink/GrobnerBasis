#include "pch.h"
#include "../GrobnerBasisLib/DegLexTermOrder.h"
#include "TermOrderTest.h"

//test the degree lexicographical term order
class DegLexTermOrderTest : public testing::Test, public TermOrderTest
{
protected:
	DegLexTermOrder deglex;
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