#include "DegLexTermOrder.h"
#include <numeric>
#include <algorithm>
using namespace std;

bool DegLexTermOrder::compare(const vector<int>& lDegrees, const vector<int>& rDegrees) const
{
	int lTotalDegree = accumulate(lDegrees.begin(), lDegrees.end(), 0); //sum of degrees
	int rTotalDegree = accumulate(rDegrees.begin(), rDegrees.end(), 0); 
	if (lTotalDegree != rTotalDegree) //compare total degree
		return lTotalDegree < rTotalDegree;
	return lexicographical_compare( //break ties with lex
		lDegrees.begin(), lDegrees.end(),
		rDegrees.begin(), rDegrees.end()
	);
}