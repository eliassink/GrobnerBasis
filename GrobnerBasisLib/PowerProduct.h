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

/// <summary>
/// This class models a product of powers of a collection of variables, such as x^2*y^3*z.
/// There is no limit on the number of variables. These power products can be multiplied, 
/// divided, and exponentiated. Pairs of power products have a least common multiple. 
/// They be compared by concrete subclasses of TermOrder, and converted to strings using a Printer.
/// Used to build polynomials.
/// </summary>
class PowerProduct final
{
public:
	/// <summary>
	/// Constructs the power product 1.
	/// </summary>
	explicit PowerProduct() = default;

	/// <summary>
	/// Constructs the power product x_n, the nth variable.
	/// </summary>
	/// <param name="n">The index of the variable, starting from 0.</param>
	explicit PowerProduct(size_t n) : mDegrees(n + 1) { mDegrees[n] = 1; }

	bool operator==(const PowerProduct& right) const { return mDegrees == right.mDegrees; }
	bool operator!=(const PowerProduct& right) const { return !(*this == right); }

	/// <summary>
	/// Returns the product of this and right.
	/// </summary>
	PowerProduct operator*(const PowerProduct& right) const;

	/// <summary>
	/// Divide two power products. Throws if isDivisibleBy(right) is false.
	/// </summary>
	PowerProduct operator/(const PowerProduct& right) const;

	PowerProduct& operator*=(const PowerProduct& right) { return *this = *this * right; }
	PowerProduct& operator/=(const PowerProduct& right) { return *this = *this / right; }

	/// <summary>
	/// Raise to an exponent.
	/// </summary>
	/// <param name="power">- the exponent</param>
	/// <returns>the result</returns>
	PowerProduct pow(int power) const;

	/// <summary>
	/// Returns true if each degree of this is greater than or equal to the corresponding
	/// degree of divisor.
	/// </summary>
	bool isDivisibleBy(const PowerProduct& divisor) const;

	/// <summary>
	/// Returns the least common multiple of this and other
	/// </summary>
	PowerProduct lcm(const PowerProduct& other) const;

	/// <summary>
	/// Convert to a string.
	/// </summary>
	/// <param name="printer">- the printer to use</param>
	template<typename Coef>
	std::string toString(Printer<Coef>& printer) const
	{
		//mDegrees itself can stay completely private, hiding all implementation details
		return printer.powerProductString(mDegrees);
	}

	/// <summary>
	/// Interface for comparing power products. 
	/// </summary>
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
	//the degree of each variable. Never has trailing zeroes.
	//if the first three variables are x,y,z, x*z^3 corresponds to {1,0,3}
	std::vector<int> mDegrees;
};
