/*
Author: Elias Sink
Date: 5/12/2022
Notes: See StreamPrinterUML.txt for Unified Modeling Language description
*/

#pragma once
#include "Printer.h"
#include <initializer_list>
#include <sstream>
#include <vector>
#include <regex>

// Prints polynomials using a std::ostringstream. Coef must have an appropriate insertion operator.
// Supports customizable variable names.
template<typename Coef>
class StreamPrinter final :
    public Printer<Coef>
{
public:
    // Constructs a StreamPrinter with default variable names x1,x2,...
    StreamPrinter() {};

    // Constructs a StreamPrinter with the variable names in the initializer list.
    // Further variables have default names.
    StreamPrinter(std::initializer_list<std::string> varNames)
        : mVarNames{ varNames } {}

    //  Constructs a StreamPrinter with the variable names in the range [first,last).
    template<typename StringIterator>
    StreamPrinter(StringIterator first, StringIterator last)
        : mVarNames{ first, last } {}

    // Add a term to the buffer.
    void addTerm(const Coef& coef, const PowerProduct& powerProduct) override
    {
        if (!isZero)
            mStringStream << " + "; 
        else
            isZero = false;
        mStringStream << coef << powerProduct.toString(*this);
    }
   
    // Return a polynomial string from the terms in the buffer, and clear the buffer.
    std::string print()
    {
        if (isZero)
            return "0";
        std::string output = mStringStream.str();
        mStringStream.str(""); //clear buffer
        isZero = true;

        //replaces "x + -y" with "x - y"
        output = std::regex_replace(output, std::regex("\\+ \\-"), "- ");
        //replaces "x + 1*y" with "x + y"
        output = std::regex_replace(output, std::regex(" 1\\*"), " ");
        //replaces "1*x + y" with "x + y"
        output = std::regex_replace(output, std::regex("^1\\*"), "");
        //replaces "-1*x + y" with "-x + y"
        output = std::regex_replace(output, std::regex("^\\-1\\*"), "-");
        return output;
    }

    // Returns a string representation of a power product with the
    // given degrees.
    std::string powerProductString(const std::vector<int>& degrees) override
    {
        std::ostringstream ss;
        for (size_t n = 0; n < degrees.size(); ++n)
        {
            if (degrees[n] == 0)
                continue;
            ss << '*' << var(n); //nth variable name
            if (degrees[n] > 1)
                ss << '^' << degrees[n];
        }
        return ss.str();
    }

private:
    //the stringstream for buffering terms
    std::ostringstream mStringStream;
    //the custom variable names
    std::vector<std::string> mVarNames;
    //true of there are no terms buffered
    bool isZero = true;
    
    //returns the nth variable name
    std::string var(size_t n) const
    {
        if (n < mVarNames.size()) 
            return mVarNames[n];//custom name
        std::ostringstream ss;
        ss << 'x' << (n + 1);
        return ss.str(); //default name
    }
};

