/*
Author: Elias Sink
Date: 5/11/2022
Notes: See PowerProductUML for a Unified Modeling Language description of this class. 
*/

#pragma once
#include <vector>
#include <stdexcept>
#include <memory>
#include <initializer_list>
#include "Printer.h"

// This class models a product of powers of a collection of variables, such as x^2*y^3*z.
// There is no limit on the number of variables. These power products can be multiplied, 
// divided, and exponentiated. Pairs of power products have a least common multiple. 
// They be compared by concrete subclasses of TermOrder, and converted to strings using a Printer.
// Used to build polynomials.
class PowerProduct final
{
public:
	// Constructs the power product 1.
	explicit PowerProduct() = default;

	// Constructs the power product x_n, the nth variable.
	explicit PowerProduct(size_t n) : mDegrees(n + 1) { mDegrees[n] = 1; }

	bool operator==(const PowerProduct& right) const { return mDegrees == right.mDegrees; }
	bool operator!=(const PowerProduct& right) const { return !(*this == right); }

	// Returns the product of this and right.
	PowerProduct operator*(const PowerProduct& right) const;

	// Divide two power products. Throws if isDivisibleBy(right) is false.
	PowerProduct operator/(const PowerProduct& right) const;

	PowerProduct& operator*=(const PowerProduct& right) { return *this = *this * right; }
	PowerProduct& operator/=(const PowerProduct& right) { return *this = *this / right; }

	// Raise to an exponent.
	PowerProduct pow(int power) const;

	// Returns true if each degree of this is greater than or equal to the corresponding
	// degree of divisor.
	bool isDivisibleBy(const PowerProduct& divisor) const;

	// Returns the least common multiple of this and other
	PowerProduct lcm(const PowerProduct& other) const;

	// Convert to a string using a Printer.
	template<typename Coef>
	std::string toString(Printer<Coef>& printer) const
	{
		//mDegrees itself can stay completely private, hiding all implementation details
		return printer.powerProductString(mDegrees);
	}

	// Interface for comparing power products. 
	class TermOrder
	{
	public:
		//Returns true if left < right.
		bool operator() (const PowerProduct& left, const PowerProduct& right) const
		{
			//delegate to a private virtual method
			return compare(left.mDegrees, right.mDegrees);
		}
	private:
		//Compares vectors of degrees, returning true if the first argument is less than the second
		virtual bool compare(const std::vector<int>& lDegrees, const std::vector<int>& rDegrees) const = 0;
	};

private:
	//The degree of each variable. Never has trailing zeroes.
	//if the first three variables are x,y,z, x*z^3 corresponds to {1,0,3}
	std::vector<int> mDegrees;
};
