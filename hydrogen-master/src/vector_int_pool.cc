#include "vector_int_pool.hh"

#include "utils.hh"

#include <iostream>

using namespace ga;

VectorIntPool::VectorIntPool()
{
	sz_elements = 0;
	cleared = true;
}

VectorIntPool::VectorIntPool(int _sz_elements)
{
	init(sz_elements);
}

// ~Pool() gets called automatically after this method body
VectorIntPool::~VectorIntPool()
{
	clear();
}

int VectorIntPool::size()
{
	return sz_elements;
}

std::vector<int> VectorIntPool::get(int ix_element)
{
	//return Pool::get(ix_element);
	if(ix_element >= size())
		throw std::runtime_error("pool bounds");
	return elements[ix_element];
}

void VectorIntPool::init(int _sz_elements)
{
	clear();
	sz_elements = _sz_elements;
	elements = std::vector<std::vector<int> >();
	elements.reserve(sz_elements);
	cleared = false;

	FORix(sz_elements) {
		std::vector<int> vi;
		vi.reserve(ix);
		FORjx(ix)
			vi.push_back(0);
		elements.push_back(vi);
	}
}

void VectorIntPool::clear()
{
	if(cleared) return;
	for(auto element : elements)
		element.clear();
	cleared = true;
}


