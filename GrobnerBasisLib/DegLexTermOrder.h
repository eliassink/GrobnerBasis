/*
Author: Elias Sink
Date: 5/12/2022
*/

#pragma once
#include "PowerProduct.h"

// Compares power products using the degree lexicographical order: compare total degree first,
// and break ties with lexicogrpahical order.
class DegLexTermOrder final :
    public PowerProduct::TermOrder
{
    //compare vectors of degrees
    bool compare(const std::vector<int>& degrees, const std::vector<int>& otherDegrees) const override;
};

