/*
Author: Elias Sink
Date: 5/11/2022
Notes: If term ordering is decoupled from Polynomial, a TermOrder would be
injected into each Ideal to perform the necessary comparisons.
*/
#pragma once
#include "Polynomial.h"
#include "Printer.h"
#include <list>
#include <queue>
#include <algorithm>

/// <summary>
/// An ideal of polynomials. Given a generating set, this class will
/// compute the reduced Grobner basis for it with respect to the given
/// term order. It can then decide membership and reduce polynomials with respect to this basis.
/// </summary>
/// <typeparam name="Coef">The coefficeint type for the polynomials.</typeparam>
/// <typeparam name="Ord">A concrete subclass of PowerProduct::TermOrder to compare terms</typeparam>
template <class Coef, class Ord>
class Ideal
{
public:
	/// <summary>
	/// Construct the zero ideal.
	/// </summary>
	Ideal() = default;

	/// <summary>
	/// Construct the ideal generated by the polynomials in the range [first,last).
	/// </summary>
	template <typename PolynomialIterator>
	Ideal(PolynomialIterator first, PolynomialIterator last)
	{
		for (auto it = first; it != last; ++it)
			if (*it != 0)
				mGrobner.push_back(*it);

		computeGrobnerBasis();
		minimizeGrobnerBasis();
		reduceGrobnerBasis();
	}

	/// <summary>
	/// Construct the ideal generated by the polynomials in the initializer list.
	/// </summary>
	Ideal(std::initializer_list<Polynomial<Coef, Ord>> init)
		: Ideal(init.begin(), init.end()) {}

	/// <summary>
	/// Reduce p with respect to the Grobner basis of the ideal.
	/// </summary>
	Polynomial<Coef,Ord> reduce(Polynomial<Coef,Ord> p) const;

	/// <summary>
	/// Return true if p is a member of the ideal, i.e., reduce(p) == 0.
	/// </summary>
	bool contains(const Polynomial<Coef, Ord>& p) const { return reduce(p) == 0; }

	/// <summary>
	/// Return true if this and right are equal, meaning they have the same
	/// Grobner basis.
	/// </summary>
	bool operator==(const Ideal& right) const
	{
		return std::is_permutation(
			mGrobner.begin(), mGrobner.end(),
			right.mGrobner.begin(), right.mGrobner.end()
		);
	}
	bool operator!=(const Ideal& right) const
	{
		return !(*this == right);
	}

	/// <summary>
	/// Conver this ideal to a string
	/// </summary>
	/// <param name="printer">- the printer to use</param>
	std::string toString(Printer<Coef>& printer);

private:
	using P = Polynomial<Coef, Ord>;
	using Basis = std::list<Polynomial<Coef, Ord>>; //linked list for simple insertion/deletion

	// The Grobner basis of this ideal.
	Basis mGrobner;

	//Compute the grobner basis using Buchberger's algorithm.
	void computeGrobnerBasis();

	//Discard redundant terms, creating a minimal Grobner basis.
	void minimizeGrobnerBasis();

	//Compute the reduced Grobner basis from the minimal one.
	void reduceGrobnerBasis();

	//Compute the S-polynomial of f and g used in Buchberger's algorithm.
	P sPoly(const P& f, const P& g);
};

template<class Coef, class Ord>
Polynomial<Coef,Ord> Ideal<Coef, Ord>::reduce(Polynomial<Coef, Ord> p) const
{
	//multivariate division algorithm; see companion paper for a prose description
	P remainder{};
	auto dividesLeadingTerm = [&p](P f) {return p.leadingPower().isDivisibleBy(f.leadingPower()); };
	while (p != 0)
	{
		//look for a basis element that divides the leading term of p
		auto match = std::find_if(mGrobner.begin(), mGrobner.end(), dividesLeadingTerm);
		if (match != mGrobner.end()) //if one was found
			p -= (p.leadingTerm() / match->leadingTerm()) * *match; //cancel the leading term
		else
		{
			remainder += p.leadingTerm(); //add the leading term to the remainder
			p -= p.leadingTerm();
		}
	}

	return remainder;
}

template<class Coef, class Ord>
std::string Ideal<Coef, Ord>::toString(Printer<Coef>& printer)
{
	//format: ( x^2 + 1, y*z )
	std::ostringstream ss;
	ss << "( ";
	for (auto it = mGrobner.begin(); it != mGrobner.end(); ++it)
	{
		if (it != mGrobner.begin())
			ss << " , ";
		ss << it->toString(printer);
	}
	ss << " )";
	return ss.str();
}

template<class Coef, class Ord>
void Ideal<Coef, Ord>::computeGrobnerBasis()
{
	//Buchberger's algorithm; see companion paper for explanation
	std::queue<std::pair<P, P>> pairs; //pairs of elements of the current basis
	for (auto second = mGrobner.begin(); second != mGrobner.end(); ++second)
		for (auto first = mGrobner.begin(); first != second; ++first)
			pairs.push({ *first,*second });
	while (!pairs.empty())
	{
		auto pair = pairs.front();
		pairs.pop();
		P h = reduce(sPoly(pair.first, pair.second)); 
		if (h != 0)
		{
			for (const auto& g : mGrobner)
				pairs.push({ g,h });
			mGrobner.push_back(h);
		}
	}
}

template<class Coef, class Ord>
void Ideal<Coef, Ord>::minimizeGrobnerBasis()
{
	for (auto it1 = mGrobner.begin(); it1 != mGrobner.end(); ++it1)
		for (auto it2 = mGrobner.begin(); it2 != mGrobner.end(); )
		{
			//delete redundant elements
			if (it1 != it2 && it1->leadingPower().isDivisibleBy(it2->leadingPower()))
			{
				it1 = mGrobner.erase(it1); 
				it2 = mGrobner.begin();
			}
			else
				++it2;
		}
	for (P& p : mGrobner)
		p /= p.leadingCoef(); //we want a monic basis
}

template<class Coef, class Ord>
void Ideal<Coef, Ord>::reduceGrobnerBasis()
{
	for (auto it = mGrobner.begin(); it != mGrobner.end(); )
	{
		auto g = *it;
		it = mGrobner.erase(it);
		mGrobner.insert(it, reduce(g)); //replace each element with its reduction by the others
	}
}

template<class Coef, class Ord>
Polynomial<Coef,Ord> Ideal<Coef, Ord>::sPoly(const P& f, const P& g)
{
	//see companion paper for the reasoning behind this formula
	P lcm = f.leadingPower().lcm(g.leadingPower());
	return (lcm / f.leadingTerm()) * f - (lcm / g.leadingTerm()) * g;
}