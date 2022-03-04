
#include <cmath>
#include "combination_generator.hh"

#include <iostream>

using namespace ga;

CombinationGenerator::CombinationGenerator(int _n)
{
	n = _n;
	// max_index = std::pow(2, n);
	//std::cout << "max index :" << max_index << std::endl;
}

// CombinationGenerator::~CombinationGenerator()
// {
	
// }

CombinationIterator* CombinationGenerator::begin()
{
	return new CombinationIterator(n);
}

CombinationIterator* CombinationGenerator::end()
{
	return new CombinationIterator(n);
}
