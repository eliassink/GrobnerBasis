/*
Author: Elias Sink
Date: 5/12/2022
Notes: Weird that there's no standard library component for this, 
but I guess that's C++.
*/

#pragma once
#include <stdexcept>
#include <iostream>

/// <summary>
/// Models the rational numbers, with arithmetic, comparison, and stream insertion/extraction 
/// </summary>
/// <typeparam name="Integer">
/// The integral type to use. Must support arithmetic, comparison, and stream insertion/extraction
/// </typeparam>
template<typename Integer>
class Rational final
{
public:
	/// <summary>
	/// Constructs a rational number.
	/// </summary>
	Rational(Integer numerator = 0, Integer denominator = 1)
		: mNumerator{numerator}, mDenominator{denominator}
	{
		if (denominator == 0)
			throw std::logic_error("denominator was zero");
		reduce();
	}

	friend bool operator==(const Rational& left, const Rational& right)
	{
		return left.mNumerator == right.mNumerator && left.mDenominator == right.mDenominator;
	}

	friend bool operator!=(const Rational& left, const Rational& right)
	{
		return !(left == right);
	}

	friend Rational operator+(const Rational& left, const Rational& right)
	{
		return Rational(
			left.mNumerator * right.mDenominator + right.mNumerator * left.mDenominator,
			left.mDenominator * right.mDenominator
		);
	}

	Rational operator-() const
	{
		return Rational(-mNumerator, mDenominator);
	}

	friend Rational operator-(const Rational& left, const Rational& right)
	{
		return left + (-right);
	}

	friend Rational operator*(const Rational& left, const Rational& right)
	{
		return Rational(
			left.mNumerator * right.mNumerator,
			left.mDenominator * right.mDenominator
		);
	}

	friend Rational operator/(const Rational& left, const Rational& right)
	{
		return Rational(
			left.mNumerator * right.mDenominator,
			left.mDenominator * right.mNumerator
		);
	}

	Rational& operator+=(const Rational& right) { return *this = *this + right; }
	Rational& operator-=(const Rational& right) { return *this = *this - right; }
	Rational& operator*=(const Rational& right) { return *this = *this * right; }
	Rational& operator/=(const Rational& right) { return *this = *this / right; }

	friend bool operator<(const Rational& left, const Rational& right)
	{
		return left.compare(right) < 0;
	}

	friend bool operator>(const Rational& left, const Rational& right)
	{
		return left.compare(right) > 0;
	}

	friend bool operator<=(const Rational& left, const Rational& right)
	{
		return left.compare(right) <= 0;
	}

	friend bool operator>=(const Rational& left, const Rational& right)
	{
		return left.compare(right) >= 0;
	}

	friend std::ostream& operator<<(std::ostream& os, const Rational& rational)
	{
		os << rational.mNumerator;
		if (rational.mNumerator != 0 && rational.mDenominator != 1)
			os << '/' << rational.mDenominator;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Rational& rational)
	{
		Integer num = 0, den = 1;
		is >> num;
		if (is.peek() == '/')
		{
			is.ignore();
			is >> den;
		}
		rational = { num, den };
		return is;
	}
private:
	Integer mNumerator;
	Integer mDenominator;
	//absolute value of an integer
	Integer abs(Integer x) const
	{
		if (x < 0)
			return -x;
		else
			return x;
	}

	//reduce to lowest terms with positive denominator
	void reduce() {
		if (mDenominator == 1)
			return;
		if (mDenominator < 0) //fix sign
		{
			mNumerator *= -1;
			mDenominator *= -1;
		}
		Integer x = abs(mNumerator); //Euclidean algorithm
		Integer y = mDenominator;
		while (y != 0)
		{
			Integer temp = y;
			y = x % y;
			x = temp;
		}
		mNumerator /= x;
		mDenominator /= x;
	}

	//positive if *this > right, negtive if *this < right, 0 if *this == right
	Integer compare(const Rational& right) const
	{
		return mNumerator * right.mDenominator - mDenominator * right.mNumerator;
	}
};

