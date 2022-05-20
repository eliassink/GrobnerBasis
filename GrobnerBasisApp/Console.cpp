/*
Author: Elias Sink
Date: 5/12/2022
*/

#include "Console.h"

//printed by the "help" command
const std::string helpString =
"\nSolves the ideal membership problem for polynomials in x,y,z with rational coefficients.\n"
"Polynomials should be entered in standard form (1/2*x^2 + x*y + 1/2*y^2, not 1/2*(x+y)^2).\n\n"
"COMMANDS:\n\n"
"help\n"
"Display this help menu.\n\n"
"ideal p1,p2,...\n"
"Set the generators of the ideal and print its Grobner basis (lex term order).\n"
"Example:\n"
">>ideal x^2*y - x + 1, -y^2*z + 1/3*x^3\n"
"I := ( y*x^2 - x + 1 , z*y^2 - 1/3*x^3 , x^5 - 3*z*y*x + 3 * z * y )\n\n"
"member p\n"
"Print true if p is in the ideal. Otherwise, print false.\n"
"Example:\n"
">>member x^5 - 3*z*y*x + 3 * z * y\n"
"true\n\n"
"reduce p\n"
"Print the reduction of p by the ideal (lex term order).\n"
"Example:\n"
">>reduce y^2*x^3\n"
"-y*x + x - 1\n\n"
"quit\n"
"Quits the application.\n\n";

//prints at startup
const std::string headerString =
"IDEAL MEMBERSHIP SOLVER\n"
"Enter \"help\" for instructions, or \"quit\" to exit.\n>>"; //includes >> prompt


std::string Console::header()
{
	return headerString;
}

std::string Console::dispatchCommand(const std::string& commandLine)
{
	std::istringstream input(commandLine); //wrap the line in a stringstream
	std::ostringstream output;
	output << std::boolalpha;

	std::string command;
	input >> command; //get the first token, the name of the command

	if (command == "quit")
		mQuit = true;
	else if (command == "ideal")
		setIdeal(input, output);
	else if (command == "member")
		isMember(input, output);
	else if (command == "reduce")
		reduce(input,output);
	else if (command == "help")
		output << helpString;
	else if (command != "") //if blank, do nothing
		output << "Unknown command " << command << "\n";

	if (!mQuit) 
		output << ">>";
	return output.str();
}

void Console::setIdeal(std::istream& input, std::ostream& output)
{
	try
	{
		std::vector<Polynomial<Rational<int>, LexTermOrder>> gens; //generators of the ideal

		//read the comma-separated list of polynomials
		for (std::string polyString; std::getline(input, polyString, ',');)
			gens.push_back(mParser.parse(polyString)); 

		//set the ideal
		mIdeal = { gens.begin(), gens.end() };

		//print back the ideal as a Grobner basis
		output << "I := " << mIdeal.toString(mPrinter) << "\n";
	}
	catch (std::exception& ex)
	{
		output << "Error: " << ex.what() << "\n"; 
	}
}

void Console::isMember(std::istream& input, std::ostream& output)
{
	try
	{
		std::string polyString;
		std::getline(input, polyString); //get the rest of the line
		output << mIdeal.contains(mParser.parse(polyString)) << "\n"; //parse and answer
	}
	catch (std::exception& ex)
	{
		output << "Error: " << ex.what() << "\n";
	}
}
void Console::reduce(std::istream& input, std::ostream& output)
{
	try
	{
		std::string polyString;
		std::getline(input, polyString); //get the rest of the line
		output << mIdeal.reduce(mParser.parse(polyString)).toString(mPrinter) << "\n";//parse and answer
	}
	catch (std::exception& ex)
	{
		output << "Error: " << ex.what() << "\n";
	}
}