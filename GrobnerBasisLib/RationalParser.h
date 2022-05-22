/*
Author: Elias Sink
Date: 5/11/2022
Notes: If term ordering were decoupled from Polynomial, it would be decoupled from here as well.
This parser is pretty simplistic; ideally it would be able to handle inputs like (x+y)^2, but
that turns out to be pretty hard.
*/


#pragma once
#include "Parser.h"
#include "Polynomial.h"
#include "Rational.h"
#include <regex>
#include <vector>
#include <sstream>
#include <algorithm>

// Parses polynomials with rational coefficients in standard form.
class RationalParser final :
    public Parser<Polynomial<Rational<int>>>
{
public:
	using Parser<Polynomial<Rational<int>>>::ParseException; //bring into this namespace

	template <typename StringIterator>
	// Construct a parser with variable names in the range [first,last).
	RationalParser(StringIterator first, StringIterator last)
		: mVarNames{ first,last } 
	{
		verifyNames();
	}

	// Construct a parser with variable names from an initializer list.
	RationalParser(std::initializer_list<std::string> varNames)
		: mVarNames{ varNames }
	{
		verifyNames();
	}

    // Parse a polynomial from the input string. Throws Parser<>::ParseException if an error occurs.
    Polynomial<Rational<int>> parse(const std::string& input) const override;

private:

	//the names of the variables recognized by the parser, in order.
	std::vector<std::string> mVarNames;

	//check that the names are valid: 1 or more letters followed by 0 or more numbers
	void verifyNames() const;

	//parse a single term
	Polynomial<Rational<int>> parseTerm(const std::string& termString) const;
};
