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
#include "Ideal.h"
#include "RationalParser.h"
#include "StreamPrinter.h"

class Console
{
public:
	// Constructs a Console using the variable names in
	// the range [first,last).
	template<typename StringIterator>
	Console(StringIterator first, StringIterator last)
		: mPrinter{ first, last }, mParser{ first, last } {}

	// Constructs a Console using the variable names in
	// the initializer list.
	Console(std::initializer_list<std::string> varNames)
		: mPrinter{ varNames }, mParser{ varNames } {}

	// Returns false after the quit commmand has been issued.
	operator bool() { return !mQuit; }

	// Returns a header string to display at startup.
	std::string header();

	// Processes a command and returns a response. 
	std::string dispatchCommand(const std::string& commandLine);

private:
	StreamPrinter<Rational<>> mPrinter; //to print polynomials, etc.
	RationalParser mParser; //to parse polynomials
	Ideal<Rational<>> mIdeal; //the current ideal being considered
	bool mQuit{ false }; //true if the quit command has been issued

	void setIdeal(std::istream& input, std::ostream& output); //sets the current ideal
	void isMember(std::istream& input, std::ostream& output); //decideds membership
	void reduce(std::istream& input, std::ostream& output); //reduces a polynomial
	void setTermOrder(std::istream& input, std::ostream& output); //sets term order
};

