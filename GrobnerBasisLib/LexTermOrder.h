#pragma once
#include "PowerProduct.h"

// Compare power products lexicographically in their degree vectors.
class LexTermOrder :
    public PowerProduct::TermOrder
{
    bool compare(const std::vector<int>& leftDegrees, const std::vector<int>& rightDegrees) const override;
};