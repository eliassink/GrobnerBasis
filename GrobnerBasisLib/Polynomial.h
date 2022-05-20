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

//the default term order for polynomials
using DefaultTermOrder = LexTermOrder;

/// <summary>
/// A polynomial is a sum of coefficients times product powers. They may be added, subtracted,
/// multiplied, raised to exponents, and divided by monomials. The terms of a polynomial are 
/// ordered (via Ord), and the leading term can be queried. Conversion to strings is done
/// via a Printer. PowerProducts, Coef, and int may all be implicitly converted to Polynomial
/// in an arithmetic expression (most operator overloads are non-member to support this).
/// </summary>
/// <typeparam name="Coef">
/// The coefficient type. Must support all arithmetic operations (incl. division) and 
/// construction/casting from int.
/// </typeparam>
/// <typeparam name="Ord">
/// The subclass of PowerProduct::TermOrder used to compare terms.
/// </typeparam>
template <typename Coef, typename Ord = DefaultTermOrder>
class Polynomial final
{
public:
	/// <summary>
	/// Constructs a zero polynomial.
	/// </summary>
	Polynomial() = default;

	/// <summary>
	/// Constructs a monic monomial from a power product.
	/// </summary>
	Polynomial(const PowerProduct& powerProduct)
		: mTerms{ {powerProduct,Coef(1)} } { }

	/// <summary>
	/// Constructs a constant polynomial.
	/// </summary>
	/// <param name="constant">- the value of the constant</param>
	Polynomial(const Coef& constant)
	{
		if (constant != Coef(0)) //if constant is zero, there are no terms
			mTerms.insert({ PowerProduct(), constant});
	}

	/// <summary>
	/// Constructs a constant polynomial from an integer.
	/// </summary>
	/// <param name="constant">- the value of the constant</param>
	Polynomial(int constant) 
		: Polynomial(Coef(constant)) {}

	friend bool operator==(const Polynomial& left, const Polynomial& right)
	{
		return left.mTerms == right.mTerms;
	}
	friend bool operator!=(const Polynomial& left, const Polynomial& right)
	{
		return !(left == right);
	}

	/// <summary>
	/// Add two polynomials.
	/// </summary>
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

	/// <summary>
	/// Negate a polynomial.
	/// </summary>
	friend Polynomial operator-(const Polynomial& arg)
	{
		Polynomial negation{ arg };
		for (auto& term : negation.mTerms)
			term.second = -term.second; //negate each term
		return negation;
	}

	/// <summary>
	/// Subtract two polynomials.
	/// </summary>
	friend Polynomial operator-(const Polynomial& left, const Polynomial& right)
	{
		return left + (-right);
	}

	/// <summary>
	/// Multiply two polynomials.
	/// </summary>
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
	/// <summary>
	/// Divide a polynomial by a monomial.
	/// </summary>
	/// <param name="dividend">- the dividend polynomial</param>
	/// <param name="monomial">- the divisor. Must have exactly one term.</param>
	/// <returns>the quotient</returns>
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

	/// <summary>
	/// Raise the polynomial to a power.
	/// </summary>
	Polynomial pow(int power) const;

	/// <summary>
	/// Returns true if monomial has only one term which divides every term of this polynomial
	/// </summary>
	bool isDivisibleBy(const Polynomial& monomial) const;

	/// <summary>
	/// Returns the leading power product of this polynomial, as determined by Ord.
	/// </summary>
	PowerProduct leadingPower() const;

	/// <summary>
	/// Returns the leading coeficient of this polynomial, as determined by Ord.
	/// </summary>
	Coef leadingCoef() const;
	
	/// <summary>
	/// Returns the leading term of this polynomial, as determined by Ord.
	/// </summary>
	Polynomial leadingTerm() const;

	//TO CONSIDER: Proposed replacements for dynamic ordering
	//PowerProduct leadingPower(TermOrder& termOrder);  
	//Coef leadingCoef(TermOrder& termOrder);
	//Polynomial leadingTerm(TermOrder& termOrder);

	/// <summary>
	/// Converts this polynomial to a string.
	/// </summary>
	/// <param name="printer">- the printer to use</param>
	std::string toString(Printer<Coef>& printer) const;

private:
	//The terms of this polynomial, as a map from power products to coefficients.
	//That is, mTerms[p] is the coefficient of power product p.
	//Sorted with respect to Ord.
	//Entries with value zero are always removed.
	std::map<PowerProduct, Coef, Ord> mTerms;

	//strip off any zero terms
	void simplify();

	//exponentiate this polynomial recursively
	Polynomial recursivePow(int power) const;
};


template <typename Coef, typename Ord>
bool Polynomial<Coef, Ord>::isDivisibleBy(const Polynomial& monomial) const
{
	if (monomial.mTerms.size() != 1) //divisor must have exactly one term
		return false;
	for (const auto& term : mTerms) //each term of the dividend must be divisible by that term
		if (!term.first.isDivisibleBy(monomial.mTerms.begin()->first)) 
			return false;
	return true;
}

template <typename Coef, typename Ord>
Polynomial<Coef, Ord> Polynomial<Coef, Ord>::pow(int power) const
{
	if (power < 0)
		throw std::logic_error("polynomial raised to negative exponent");

	if (mTerms.size() == 1 && mTerms.begin()->second == Coef(1)) 
		return mTerms.begin()->first.pow(power); //optimization for standalone power products
	else
		return recursivePow(power); //recursive algorithm for general case
}

template <typename Coef, typename Ord>
PowerProduct Polynomial<Coef,Ord>::leadingPower() const
{
	if (mTerms.empty())
		throw std::logic_error("leadingPower called on 0"); //leading power product of 0 is undefined
	else
		return mTerms.rbegin()->first; //the last term in the map is the leading term 
}

template <typename Coef, typename Ord>
Coef Polynomial<Coef, Ord>::leadingCoef() const
{
	if (mTerms.empty())
		return Coef(0); //leading coefficient of 0 is 0
	else
		return mTerms.rbegin()->second;
}

template <typename Coef, typename Ord>
Polynomial<Coef,Ord> Polynomial<Coef, Ord>::leadingTerm() const
{
	if (mTerms.empty())
		return Coef(0); //leading term of 0 is 0
	else
		return leadingCoef() * Polynomial(leadingPower());
}

template <typename Coef, typename Ord>
std::string Polynomial<Coef, Ord>::toString(Printer<Coef>& printer) const
{
	for (auto it = mTerms.rbegin(); it != mTerms.rend(); ++it)
		printer.addTerm(it->second, it->first); //add terms in reverse order, so leading term is first
	return printer.print();
}

template<typename Coef, typename Ord>
void Polynomial<Coef, Ord>::simplify()
{
	for (auto it = mTerms.begin(); it != mTerms.end();)
	{
		if (it->second == Coef(0))
			it = mTerms.erase(it); //erase terms with coefficient 0
		else
			++it;
	}
}

template<typename Coef, typename Ord>
Polynomial<Coef, Ord> Polynomial<Coef, Ord>::recursivePow(int power) const
{
	if (power == 0) //base case
		return Coef(1);
	if (power == 1) 
		return *this;
	Polynomial temp = recursivePow(power / 2); //recursion
	return (power % 2 == 0) ? temp * temp : temp * temp * *this;
}