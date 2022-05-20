/*
Author: Elias Sink
Date: 5/12/2022
*/
#include <iostream>
#include <string>
#include "Console.h"

int main()
{
	Console console{ "z","y","x" }; //variable names in reverse order to look better
	
	std::cout << console.header();
	for (std::string line; console && std::getline(std::cin, line); ) //input loop
	{
		std::cout << console.dispatchCommand(line);
	}
	return 0;
}