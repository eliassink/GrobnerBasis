/*
Author: Elias Sink
Date: 5/11/2022
Notes: Possibly unnecessarily general
*/

#pragma once
#include <string>
#include <stdexcept>

// Interface for parsing a string and returning a T.
template <typename T>
class Parser
{
public:
	// Parse the given string.
	virtual T parse(const std::string& input) const = 0;

	// Indicates an exception occured while parsing.
	class ParseException : public std::runtime_error {
	public:
		explicit ParseException() {}
		explicit ParseException(std::string message) : std::runtime_error(message) {}
	};
};

