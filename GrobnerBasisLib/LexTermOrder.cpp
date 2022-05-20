/*
Author: Elias Sink
Date: 5/12/2022
*/

#include "LexTermOrder.h"
#include <algorithm>
using namespace std;

bool LexTermOrder::compare(const vector<int>& leftDegrees, const vector<int>& rightDegrees) const
{
    return lexicographical_compare(
        leftDegrees.begin(), leftDegrees.end(),
        rightDegrees.begin(), rightDegrees.end()
    );
}
