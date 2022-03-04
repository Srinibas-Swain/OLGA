
#include "bool_iterator.hh"

#include <iostream>

using namespace ga;

BoolIterator::BoolIterator()
{
	
}

void BoolIterator::init(const bool* _data, int _n)
{
	data = _data;
	in_elements = _n;
	current_index = -1;
}

bool BoolIterator::not_yet_done()
{
	return !is_done();
}

bool BoolIterator::is_done()
{
	if(current_index == -1)
		current_index = 0;
	
	while(current_index < (in_elements-1)) {
		++current_index;
		if(data[current_index])
			return false;
		}
	return true;
}

int BoolIterator::current()
{
	return current_index;
}

void BoolIterator::reset()
{
	current_index = -1;
}
