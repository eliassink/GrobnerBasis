#pragma once
#include "PowerProduct.h"
class DegRevLexTermOrder
	: public PowerProduct::TermOrder
{
	bool compare(const std::vector<int>& lDegrees, const std::vector<int>& rDegrees) const override;
};