#include "vector_fixed.hh"

#include <iostream>

using namespace ga;

VectorFixed::VectorFixed(int _size, int _invalid)
{
	// data = std::vector<int>(_size, invalid);
	size_total = _size;
	invalid_filler = _invalid;
	data = new int[_size];
	//std::cout << size_total << std::endl;
	reset();
}

VectorFixed::~VectorFixed()
{
	delete[](data);
}

void VectorFixed::reset()
{
	//std::cout << size_total << std::endl;
	FORix(size_total) {
		data[ix] = invalid_filler;
	}
	ix_current = -1;
	size_valid = 0;
}

int VectorFixed::next()
{
	++ix_current;
	if(ix_current >= size_valid) {
		//throw error
	}
	return data[ix_current];
}

// VectorFixed::iterator VectorFixed::begin()
int* VectorFixed::begin()
{
	return data;
}

int* VectorFixed::end()
{
	return data+size_valid;
}

void VectorFixed::add(int element)
{
	//std::cout << "add: " << element << " size: " << size_valid << " total: " << size_valid << std::endl;
	if(size_valid >= size_total) {
		// throw error
	}
	data[size_valid++] = element;
}

int VectorFixed::size()
{
	return size_valid;
}

void VectorFixed::print()
{
	std::cout << "{";
	FORix(size_valid-1)
		std::cout << data[ix] << ",";
	std::cout << data[size_valid-1] << "}";
}

std::string VectorFixed::get_string()
{
	std::ostringstream oss_vf;
	oss_vf << "{";
	FORix(size_valid-1)
		oss_vf << data[ix] << ",";
	oss_vf << data[size_valid-1] << "}";
	return oss_vf.str();
}

