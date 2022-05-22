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
#include <memory>
#include <sstream>
#include <functional>

// An ideal of polynomials. Given a generating set, this class will
// compute the reduced Grobner basis for it with respect to the given
// term order. It can then decide membership and reduce polynomials with respect to this basis.
template <class CoefT>
class Ideal
{
public:
	using DefaultTermOrder = LexTermOrder;

	// Construct the zero ideal.
	Ideal()
		: pTermOrder{ std::make_unique<DefaultTermOrder>() } {}

	// Constructs the ideal generated by the polynomials in the range [first,last) (default term order).
	template <typename PolyIterator>
	Ideal(PolyIterator first, PolyIterator last)
		: Ideal(first, last, std::make_unique<DefaultTermOrder>()) {}

	// Constructs the ideal generated by the polynomials in the range [first,last).
	template <typename PolyIterator>
	Ideal(PolyIterator first, PolyIterator last, std::unique_ptr<PowerProduct::TermOrder> termOrder)
	{
		for (auto it = first; it != last; ++it)
			if (*it != 0)
				mGrobner.push_back(*it);
		setTermOrder(std::move(termOrder));
	}

	// Constructs the ideal generated by the polynomials in the initializer list (default term order).
	Ideal(std::initializer_list<Polynomial<CoefT>> init)
		: Ideal(init.begin(), init.end()) {}

	// Constructs the ideal generated by the polynomials in the initializer list
	Ideal(std::initializer_list<Polynomial<CoefT>> init, std::unique_ptr<PowerProduct::TermOrder> termOrder)
		: Ideal(init.begin(), init.end(), std::move(termOrder)) { }

	void setTermOrder(std::unique_ptr<PowerProduct::TermOrder> termOrder) {
		if (!termOrder)
			throw std::logic_error("null term order");
		pTermOrder = std::move(termOrder);

		computeGrobnerBasis();
		minimizeGrobnerBasis();
		reduceGrobnerBasis();
	}

	// Reduces p with respect to the Grobner basis of the ideal.
	Polynomial<CoefT> reduce(Polynomial<CoefT> p) const;

	// Returns true if p is a member of the ideal, i.e., reduce(p) == 0.
	bool isMember(const Polynomial<CoefT>& p) const { return reduce(p) == 0; }

	// Returns true if other is contained in this ideal
	bool contains(const Ideal& other) const 
	{
		return std::all_of(
			other.mGrobner.cbegin(), other.mGrobner.cend(), 
			[&](const auto& p) {return isMember(p); }
		);
	}

	// Returns true if this and other are equal as sets of polynomials (term order is ignored).
	bool equals(const Ideal& other) const { return contains(other) && other.contains(*this); }

	// Prints the Grobner basis of this ideal using a Printer.
	std::string toString(Printer<CoefT>& printer);

private:
	using P = Polynomial<CoefT>;
	using Basis = std::list<P>; //linked list for simple insertion/deletion

	// The Grobner basis of this ideal.
	Basis mGrobner;

	// The term order to use
	std::unique_ptr<const PowerProduct::TermOrder> pTermOrder;

	//Compute the grobner basis using Buchberger's algorithm.
	void computeGrobnerBasis();

	//Discard redundant terms, creating a minimal Grobner basis.
	void minimizeGrobnerBasis();

	//Compute the reduced Grobner basis from the minimal one.
	void reduceGrobnerBasis();

	//Compute the S-polynomial of f and g used in Buchberger's algorithm.
	P sPoly(const P& f, const P& g);
};

template <class CoefT>
Polynomial<CoefT> Ideal<CoefT>::reduce(Polynomial<CoefT> p) const
{
	//multivariate division algorithm; see companion paper for a prose description
	P remainder{};
	auto dividesLeadingTerm = [&](P f) 
	{
		return p.leadingPower(*pTermOrder).isDivisibleBy(f.leadingPower(*pTermOrder)); 
	};

	while (p != 0)
	{
		//look for a basis element that divides the leading term of p
		auto match = std::find_if(mGrobner.begin(), mGrobner.end(), dividesLeadingTerm);
		if (match != mGrobner.end()) //if one was found
			p -= (p.leadingTerm(*pTermOrder) / match->leadingTerm(*pTermOrder)) * *match; //cancel the leading term
		else
		{
			remainder += p.leadingTerm(*pTermOrder); //add the leading term to the remainder
			p -= p.leadingTerm(*pTermOrder);
		}
	}

	return remainder;
}

template<class CoefT>
std::string Ideal<CoefT>::toString(Printer<CoefT>& printer)
{
	//format: ( x^2 + 1, y*z )
	std::ostringstream ss;
	ss << "( ";
	for (auto it = mGrobner.begin(); it != mGrobner.end(); ++it)
	{
		if (it != mGrobner.begin())
			ss << " , ";
		ss << it->toString(printer,*pTermOrder);
	}
	ss << " )";
	return ss.str();
}

template<class CoefT>
void Ideal<CoefT>::computeGrobnerBasis()
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

template<class CoefT>
void Ideal<CoefT>::minimizeGrobnerBasis()
{
	for (auto it1 = mGrobner.begin(); it1 != mGrobner.end(); ++it1)
		for (auto it2 = mGrobner.begin(); it2 != mGrobner.end(); )
		{
			//delete redundant elements
			if (it1 != it2 && it1->leadingPower(*pTermOrder).isDivisibleBy(it2->leadingPower(*pTermOrder)))
			{
				it1 = mGrobner.erase(it1); 
				it2 = mGrobner.begin();
			}
			else
				++it2;
		}
	for (P& p : mGrobner)
		p /= p.leadingCoef(*pTermOrder); //we want a monic basis
}

template<class CoefT>
void Ideal<CoefT>::reduceGrobnerBasis()
{
	for (auto it = mGrobner.begin(); it != mGrobner.end(); )
	{
		auto g = *it;
		it = mGrobner.erase(it);
		mGrobner.insert(it, reduce(g)); //replace each element with its reduction by the others
	}
}

template<class CoefT>
Polynomial<CoefT> Ideal<CoefT>::sPoly(const P& f, const P& g)
{
	//see companion paper for the reasoning behind this formula
	P lcm = f.leadingPower(*pTermOrder).lcm(g.leadingPower(*pTermOrder));
	return (lcm / f.leadingTerm(*pTermOrder)) * f - (lcm / g.leadingTerm(*pTermOrder)) * g;
}
