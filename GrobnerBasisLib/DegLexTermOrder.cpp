#include "DegLexTermOrder.h"
#include <numeric>
#include <algorithm>
using namespace std;

bool DegLexTermOrder::compare(const vector<int>& degrees, const vector<int>& otherDegrees) const
{
	int totalDegree = accumulate(degrees.begin(), degrees.end(), 0); //sum of degrees
	int otherTotalDegree = accumulate(otherDegrees.begin(), otherDegrees.end(), 0); 
	if (totalDegree != otherTotalDegree) //compare total degree
		return totalDegree < otherTotalDegree;
	return lexicographical_compare( //break ties with lex
		degrees.begin(), degrees.end(),
		otherDegrees.begin(), otherDegrees.end()
	);
}