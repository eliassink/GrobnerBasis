#include "RationalParser.h"

//regexes

//matches 25, 3/11, etc.
const std::string number{ "[0-9]+(\\/[0-9]+)?" };
//matches x, y, a1, Name123, etc.
const std::string name{ "[a-zA-Z]+[0-9]*" };
//matches x, x^2, a1^3, etc.
const std::string power{ name + "(\\^[0-9]+)?" };
//matches 2/3*x^2*y^3, x1^2*x2*x3^5, etc.
const std::string term{ "(" + number + "|" + power + ")(\\*" + power + ")*" };
//matches -3*x+x^2*y, x-5y, etc.
const std::string poly{ "\\-?" + term + "([\\+\\-]" + term + ")*" };

Polynomial<Rational<int>> RationalParser::parse(const std::string& input) const
{
	std::string strippedInput = std::regex_replace(input, std::regex{ "\\s" }, ""); //strip whitespace
	if (!std::regex_match(strippedInput, std::regex(poly))) //check that the format is valid
		throw ParseException("invalid input format: " + input);

	Polynomial<Rational<int>> result;

	//beginning and end of each term
	size_t first = 0, last = strippedInput.find_first_of("+-", 1);
	while (first < strippedInput.size())
	{
		if (last == std::string::npos) last = strippedInput.size(); //this is the final term

		if (strippedInput[first] == '+') //addition
			result += parseTerm(strippedInput.substr(first + 1, last - (first + 1)));
		else if (strippedInput[first] == '-') //subtraction
			result -= parseTerm(strippedInput.substr(first + 1, last - (first + 1)));
		else //first term, positive
			result += parseTerm(strippedInput.substr(first, last - first));
		first = last;
		last = strippedInput.find_first_of("+-", first + 1); //next term
	}

	return result;
}

void RationalParser::verifyNames() const
{
	for (const auto& var : mVarNames)
	{
		if (!std::regex_match(var, std::regex{ name }))
			throw ParseException("invalid variable name " + var);
		if (std::count(mVarNames.begin(), mVarNames.end(), var) > 1)
			throw ParseException("duplicate variable name " + var);
	}
}

Polynomial<Rational<int>> RationalParser::parseTerm(const std::string& termString) const
{
	Polynomial<Rational<int>> term = 1;
	std::stringstream ss(termString);
	if (std::isdigit(ss.peek())) //if there's a coefficient
	{
		Rational<int> coef;
		ss >> coef;
		term *= coef;
		ss.ignore(); //skip the *
	}

	//split the rest of the input at each '*' to get each "power" such as x^2 or a1^3
	for (std::string powerString; std::getline(ss, powerString, '*'); )
	{
		auto caretPos = powerString.find('^'); //find the caret, if any
		//find the position of the variable name in the list
		auto varPos = std::find(mVarNames.begin(), mVarNames.end(), powerString.substr(0, caretPos));
		if (varPos == mVarNames.end()) //didn't find it
			throw ParseException("unknown variable name " + powerString.substr(0, caretPos));

		int p = 1; //the power
		if (caretPos != std::string::npos) //if there was an exponent
			p = std::stoi(powerString.substr(caretPos + 1)); //parse it

		//update the term
		auto varIndex = std::distance(mVarNames.begin(), varPos);
		term *= PowerProduct(varIndex).pow(p);
	}

	return term;
}