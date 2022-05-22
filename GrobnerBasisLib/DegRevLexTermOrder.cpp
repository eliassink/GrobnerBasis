#include "DegRevLexTermOrder.h"
#include <algorithm>
#include <numeric>

bool DegRevLexTermOrder::compare(const std::vector<int>& lDegrees, const std::vector<int>& rDegrees) const
{
	int lTotalDegree = accumulate(lDegrees.begin(), lDegrees.end(), 0); //sum of degrees
	int rTotalDegree = accumulate(rDegrees.begin(), rDegrees.end(), 0);
	if (lTotalDegree != rTotalDegree) //compare total degree
		return lTotalDegree < rTotalDegree;

	if (lDegrees.size() != rDegrees.size())
		return lDegrees.size() > rDegrees.size(); //fewer trailing 0s -> less than

	return lexicographical_compare( //break ties with reverse colex
		rDegrees.rbegin(), rDegrees.rend(),
		lDegrees.rbegin(), lDegrees.rend()
	);
}
