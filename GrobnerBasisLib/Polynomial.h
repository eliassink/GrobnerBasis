/*
Author: Elias Sink
Date: 5/11/2022
Notes: Ideally, the term order could be determined at runtime, rather 
than at compile time via templates. This requires either injecting it 
into each polynomial instance (which complicates things significantly)
or passing a TermOrder as an argument to the methods that need comparisons
(like leadingTerm). I'd probably favor the latter choice in retrospect, but 
it's not a mission-critical feature and I'm out of time to refactor for now.
For now, the static polymoprhism is adequate.
*/

#pragma once
#include "PowerProduct.h"
#include "LexTermOrder.h"
#include <map>
#include <algorithm>	
#include <string>


// A polynomial is a sum of coefficients times product powers. They may be added, subtracted,
// multiplied, raised to exponents, and divided by monomials. The terms of a polynomial are 
// ordered (via Ord), and the leading term can be queried. Conversion to strings is done
// via a Printer. PowerProducts, CoefT, and int may all be implicitly converted to Polynomial
// in an arithmetic expression (most operator overloads are non-member to support this).
//
// CoefT must support arithmetic (incl. division) and casting/contruction from int. 
template <typename CoefT>
class Polynomial final
{
public:
	// Constructs a zero polynomial.
	Polynomial() = default;

	// Constructs a monic monomial from a power product.
	Polynomial(const PowerProduct& powerProduct)
		: mTerms{ {powerProduct,CoefT(1)} } { }

	// Constructs a constant polynomial.
	Polynomial(const CoefT& constant)
	{
		if (constant != CoefT(0)) //if constant is zero, there are no terms
			mTerms.insert({ PowerProduct(), constant});
	}

	// Constructs a constant polynomial from an integer.
	Polynomial(int constant) 
		: Polynomial(CoefT(constant)) {}

	friend bool operator==(const Polynomial& left, const Polynomial& right)
	{
		return left.mTerms == right.mTerms;
	}
	friend bool operator!=(const Polynomial& left, const Polynomial& right)
	{
		return !(left == right);
	}

	// Add two polynomials.
	friend Polynomial operator+(const Polynomial& left, const Polynomial& right)
	{
		//all non-member operator overloads are implemented inline to avoid
		//a huge mess of template declarations

		Polynomial sum{ left }; //start with left

		for (const auto& term : right.mTerms)
			sum.mTerms[term.first] += term.second; //add each term of right
		sum.simplify(); //remove any zero terms
		return sum;
	}

	// Negate a polynomial.
	friend Polynomial operator-(const Polynomial& arg)
	{
		Polynomial negation{ arg };
		for (auto& term : negation.mTerms)
			term.second = -term.second; //negate each term
		return negation;
	}

	// Subtract two polynomials.
	friend Polynomial operator-(const Polynomial& left, const Polynomial& right)
	{
		return left + (-right);
	}

	// Multiply two polynomials.
	friend Polynomial operator*(const Polynomial& left, const Polynomial& right)
	{
		//to mulyiply polynomials, you multiply every pair of terms and add them 
		Polynomial product;
		for (const auto& leftTerm : left.mTerms)
			for (const auto& rightTerm : right.mTerms)
				product.mTerms[leftTerm.first * rightTerm.first] += leftTerm.second * rightTerm.second;
		product.simplify();
		return product;
	}

	// Divide a polynomial by a monomial. Throws if dividend.isDivisibleBy(monomial) is false.
	friend Polynomial operator/(const Polynomial& dividend, const Polynomial& monomial)
	{
		if (!dividend.isDivisibleBy(monomial))
			throw(std::logic_error("polynomial not divisible"));
		Polynomial quotient;

		//power product and coefficient of the single term of monomial
		const auto& rPower = monomial.mTerms.begin()->first; //C++17 structured bindings?
		const auto& rCoef = monomial.mTerms.begin()->second; //don't want to mess up build
		for (auto& term : dividend.mTerms) //divide each term by the monomial
		{
			quotient +=	(term.second / rCoef) * Polynomial(term.first / rPower); 
		}
		quotient.simplify();
		return quotient;
	}

	Polynomial& operator+=(const Polynomial& right) { return *this = *this + right; }
	Polynomial& operator-=(const Polynomial& right) { return *this = *this - right; }
	Polynomial& operator*=(const Polynomial& right) { return *this = *this * right; }
	Polynomial& operator/=(const Polynomial& monomial) { return *this = *this / monomial; }

	// Raise the polynomial to a power.
	Polynomial pow(int power) const;

	// Returns true if monomial has exactly one term which divides every term of this polynomial.
	bool isDivisibleBy(const Polynomial& monomial) const;

	// Returns the leading power product of this polynomial, as determined by termOrder.
	PowerProduct leadingPower(const PowerProduct::TermOrder& termOrder) const;

	// Returns the leading coeficient of this polynomial, as determined by termOrder.
	CoefT leadingCoef(const PowerProduct::TermOrder& termOrder) const;

	// Returns the leading term of this polynomial, as determined by termOrder.
	Polynomial leadingTerm(const PowerProduct::TermOrder& termOrder) const;
	
	// Converts this polynomial to a string using a Printer.
	// Terms are printed in an unspecified order.
	std::string toString(Printer<CoefT>& printer) const;

	// Converts this polynomial to a string using a Printer.
	// Terms are printed in the specified order, greatest to least.
	std::string toString(Printer<CoefT>& printer,const PowerProduct::TermOrder& termOrder) const;

private:
	//The term order used internally.
	using Ord = LexTermOrder;

	//The terms of this polynomial, as a map from power products to coefficients.
	//That is, mTerms[p] is the coefficient of power product p.
	//Sorted with respect to Ord.
	//Entries with value zero are always removed.
	std::map<PowerProduct, CoefT, Ord> mTerms;

	//strip off any zero terms
	void simplify();

	//return an iterator to the leading term in mTerms
	auto leading(const PowerProduct::TermOrder& termOrder) const
	{
		return std::max_element(
			mTerms.begin(), mTerms.end(),
			[&](auto l, auto r) { return termOrder(l.first, r.first); }
		);
	}

	//exponentiate this polynomial recursively
	Polynomial recursivePow(int power) const;
};


template <typename CoefT>
bool Polynomial<CoefT>::isDivisibleBy(const Polynomial& monomial) const
{
	if (monomial.mTerms.size() != 1) //divisor must have exactly one term
		return false;
	for (const auto& term : mTerms) //each term of the dividend must be divisible by that term
		if (!term.first.isDivisibleBy(monomial.mTerms.begin()->first)) 
			return false;
	return true;
}

template <typename CoefT>
Polynomial<CoefT> Polynomial<CoefT>::pow(int power) const
{
	if (power < 0)
		throw std::logic_error("polynomial raised to negative exponent");

	if (mTerms.size() == 1 && mTerms.begin()->second == CoefT(1)) 
		return mTerms.begin()->first.pow(power); //optimization for standalone power products
	else
		return recursivePow(power); //recursive algorithm for general case
}

template<typename CoefT>
PowerProduct Polynomial<CoefT>::leadingPower(const PowerProduct::TermOrder& termOrder) const
{
	if (mTerms.empty())
		throw std::logic_error("leadingPower called on 0");
	else
		return leading(termOrder)->first;
}

template<typename CoefT>
CoefT Polynomial<CoefT>::leadingCoef(const PowerProduct::TermOrder& termOrder) const
{
	if (mTerms.empty())
		return CoefT(0);
	else
		return leading(termOrder)->second;
}

template<typename CoefT>
Polynomial<CoefT> Polynomial<CoefT>::leadingTerm(const PowerProduct::TermOrder& termOrder) const
{
	if (mTerms.empty())
		return CoefT(0); //leading term of 0 is 0
	else
	{
		auto it = leading(termOrder);
		return it->second * Polynomial(it->first);
	}
}

template <typename CoefT>
std::string Polynomial<CoefT>::toString(Printer<CoefT>& printer) const
{
	for (auto it = mTerms.rbegin(); it != mTerms.rend(); ++it)
		printer.addTerm(it->second,it->first); 
	return printer.print();
}

template<typename CoefT>
std::string Polynomial<CoefT>::toString(Printer<CoefT>& printer, const PowerProduct::TermOrder& termOrder) const
{
	//sort the terms according to termOrder
	std::map<PowerProduct, CoefT, const PowerProduct::TermOrder&> sortedTerms(
		mTerms.begin(),mTerms.end(),termOrder 
	);
	for (auto it = sortedTerms.rbegin(); it != sortedTerms.rend(); ++it) 
		printer.addTerm(it->second,it->first);//add terms in reverse order, so leading term is first
	return printer.print();
}

template<typename CoefT>
void Polynomial<CoefT>::simplify()
{
	for (auto it = mTerms.begin(); it != mTerms.end();)
	{
		if (it->second == CoefT(0))
			it = mTerms.erase(it); //erase terms with coefficient 0
		else
			++it;
	}
}

template<typename CoefT>
Polynomial<CoefT> Polynomial<CoefT>::recursivePow(int power) const
{
	if (power == 0) //base case
		return CoefT(1);
	if (power == 1) 
		return *this;
	Polynomial temp = recursivePow(power / 2); //recursion
	return (power % 2 == 0) ? temp * temp : temp * temp * *this;
}