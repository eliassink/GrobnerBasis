#pragma once
#include "PowerProduct.h"
/// <summary>
/// Compare power products lexicographically in their degree vectors.
/// </summary>
class LexTermOrder :
    public PowerProduct::TermOrder
{
    bool compare(const std::vector<int>& leftDegrees, const std::vector<int>& rightDegrees) const override;
};

