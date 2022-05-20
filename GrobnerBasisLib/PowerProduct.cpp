/*
Author: Elias Sink
Date: 5/12/2022
*/

#include "PowerProduct.h"
#include <stdexcept>
#include <utility>

PowerProduct PowerProduct::operator*(const PowerProduct& right) const {
	
	if (mDegrees.size() < right.mDegrees.size())
		return right.operator*(*this); //swap roles so mDegrees is bigger

	PowerProduct product{ *this }; 
	for (size_t i = 0; i < right.mDegrees.size(); ++i)
		product.mDegrees[i] += right.mDegrees[i]; //add corresponding degrees
	return product;
}

PowerProduct PowerProduct::operator/(const PowerProduct& right) const {
	if (!isDivisibleBy(right))
		throw std::logic_error("not divisible");
	
	PowerProduct quotient{ *this };

	//since isDivisibleBy(right), right.mDegrees.size() <= this->mDegrees.size()
	for (int i = 0; i < right.mDegrees.size(); ++i)
		quotient.mDegrees[i] -= right.mDegrees[i]; //subtract corresponding degrees

	while (!quotient.mDegrees.empty() && quotient.mDegrees.back() == 0)
		quotient.mDegrees.pop_back(); //remove trailing zeroes in case of cancellations
	return quotient;
}

PowerProduct PowerProduct::pow(int power) const
{
	if (power == 0)
		return PowerProduct();
	if (power < 0)
		throw std::logic_error("power product raised to negative power");

	PowerProduct result{ *this };
	for (int& d : result.mDegrees)
		d *= power; //multiply each degree by the exponent
	return result;
}

bool PowerProduct::isDivisibleBy(const PowerProduct& divisor) const {
	if (mDegrees.size() < divisor.mDegrees.size()) //variable present in the divisor but not in dividend
		return false;
	for (size_t i = 0; i < divisor.mDegrees.size(); ++i) {
		if (mDegrees[i] < divisor.mDegrees[i])
			return false;
	}
	return true;
}

PowerProduct PowerProduct::lcm(const PowerProduct& other) const
{
	if (other.mDegrees.size() > mDegrees.size())
		return other.lcm(*this); //swap roles
	PowerProduct result = *this;
	for (int i = 0; i < other.mDegrees.size(); ++i)
		result.mDegrees[i] = std::max(result.mDegrees[i], other.mDegrees[i]); //element-wise max
	return result;
}
