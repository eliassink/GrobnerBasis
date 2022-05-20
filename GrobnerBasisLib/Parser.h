/*
Author: Elias Sink
Date: 5/11/2022
Notes: Possibly unnecessarily general
*/

#pragma once
#include <string>
#include <stdexcept>

/// <summary>
/// Interface for parsing a string and returning a T.
/// </summary>
template <typename T>
class Parser
{
public:
	/// <summary>
	/// Parse the given string.
	/// </summary>
	/// <returns>the parsed value</returns>
	virtual T parse(const std::string& input) const = 0;

	/// <summary>
	/// Indicates an exception occured while parsing
	/// </summary>
	class ParseException : public std::runtime_error {
	public:
		explicit ParseException() {}
		explicit ParseException(std::string message) : std::runtime_error(message) {}
	};
};

