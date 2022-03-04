#include "pool.hh"
#include <iostream>

using namespace ga;

Pool::Pool()
{
	sz_elements = 0;
	num_deletes = 0;
	cleared = true;
}


Pool::Pool(Pool::size_type _sz_elements)
	: Pool()
{
	num_deletes = 0;
	init(_sz_elements);
}


Pool::~Pool() {
}


GenericObject& Pool::get(Pool::size_type ix_element) {
	if(ix_element >= size())
		throw std::runtime_error("pool bounds");
	return *elements[ix_element];
}


void Pool::init(Pool::size_type _sz_elements)
{
	clear();
	sz_elements = _sz_elements;
	elements = std::vector<GenericObject*>();
	elements.reserve(sz_elements);
	cleared = false;
}


void Pool::reserve(Pool::size_type _sz_elements_new)
{
	// std::vector ensures that the size is only
	// increased, never decreased.
	elements.reserve(_sz_elements_new);
}

void Pool::clear()
{
	// DEBUG3 std::cout << "pool:clear" << std::endl;
	for(auto element : elements) {
		++num_deletes;
		delete(element);
	}
	elements.clear();
	elements = std::vector<GenericObject*>();
}


Pool::size_type Pool::size() const
{
	return elements.size();
}


Pool::size_type Pool::capacity() const
{
	return elements.capacity();
}
