/*
Author: Elias Sink
Date: 5/12/2022
Notes: PowerProduct and Polynomial are passed an implementation of Printer (polymorphism!)
via their toString methods, allowing them to keep their data as private as possible. 

Because C++ requires templates for generic iterators (as opposed to Java's dynamic 
polymorphism through List or Iterator), passing a collection of data to a virtual method in a 
container-agnostic way is not truly possible. Thus, powerProductString takes a vector,
rather than an iterator range per the convention in this project. I'm not happy about this,
but the workaround was too ugly and inefficient to be worth it. PowerProduct::TermOrder::compare
has a similar issue, but at least that's private virtual
*/

#pragma once
#include <string>
#include <vector>
class PowerProduct; //forward declaration to avoid include loop

// Used by PowerProduct and Polynomial to produce string representations.
// Coef is the coefficient type of the polynomials to print. 
template<typename Coef>
class Printer
{
public:
	// Adds a term to the buffer.
	virtual void addTerm(const Coef& coef, const PowerProduct& powerProduct) = 0;

	// Returns a polynomial string from the terms in the buffer, and clears the buffer.
	virtual std::string print() = 0;

	/// Returns a string representation of a power product with the
	/// given degrees.
	virtual std::string powerProductString(const std::vector<int>& degrees) = 0;
};

