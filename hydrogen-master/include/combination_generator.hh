
#ifndef COMBINATION_GENERATOR_HH
#define COMBINATION_GENERATOR_HH

#include "combination_iterator.hh"

namespace ga {
class CombinationGenerator {
public:
	int n;
	int current_index;
	int max_index;

	CombinationGenerator(int n);
	// ~CombinationGenerator();

	CombinationIterator* begin();
	CombinationIterator* end();
};

	typedef CombinationIterator cgiterator;
}

#endif /* COMBINATION_GENERATOR_HH */
