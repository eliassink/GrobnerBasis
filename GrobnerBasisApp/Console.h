/*
Author: Elias Sink
Date: 5/12/2022
Notes: This class isn't part of the "library", so to speak; it's really just
the driver for a simplistic command line. A lot of things are hard-coded
that I'd rather inject at runtime (term order, printer, etc.), but I'm keeping
it simple for now.
*/

#pragma once
#include <sstream>
#include <vector>
#include "Ideal.h"
#include "LexTermOrder.h"
#include "Ideal.h"
#include "RationalParser.h"
#include "StreamPrinter.h"

class Console
{
public:
	/// <summary>
	/// Construct a Console using the variable names in
	/// the range [first,last).
	/// </summary>
	template<typename StringIterator>
	Console(StringIterator first, StringIterator last)
		: mPrinter{ first, last }, mParser{ first, last } {}

	/// <summary>
	/// Construct a Console using the variable names in
	/// the initializer list.
	/// </summary>
	Console(std::initializer_list<std::string> varNames)
		: mPrinter{ varNames }, mParser{ varNames } {}

	//returns false after the quit commmand has been issued.
	operator bool() { return !mQuit; }

	/// <summary>
	/// Returns a header string to display at startup.
	/// </summary>
	std::string header();

	/// <summary>
	/// Process a command.
	/// </summary>
	/// <param name="commandLine">- one line of input</param>
	/// <returns>the result of the command</returns>
	std::string dispatchCommand(const std::string& commandLine);

private:
	StreamPrinter<Rational<int>> mPrinter; //to print polynomials, etc.
	RationalParser<LexTermOrder> mParser; //to parse polynomials
	Ideal<Rational<int>, LexTermOrder> mIdeal; //the current ideal being considered
	bool mQuit{ false }; //true if the quit command has been issued

	void setIdeal(std::istream& input, std::ostream& output); //sets the current ideal
	void isMember(std::istream& input, std::ostream& output); //decideds membership
	void reduce(std::istream& input, std::ostream& output); //reduces a polynomial
};

