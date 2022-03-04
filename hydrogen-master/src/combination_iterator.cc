#include "combination_iterator.hh"

#include <cmath>
#include <iostream>

#include "utils.hh"

using namespace ga;

CombinationIterator::CombinationIterator(int _n, int ix_start)
{
	in_vertices = _n;
	max_index = std::pow(2, in_vertices);
	current_index = ix_start-1;
	init_data();
	bi = new BoolIterator();
	current_vertices = new VectorFixed(in_vertices, -1);
	init_current_vertices();
}

void CombinationIterator::init_data()
{
	data = new bool[in_vertices];
	FORix(in_vertices) {
		data[ix] = false;
	}
}

CombinationIterator::~CombinationIterator()
{
	// std::cout << "\ndeleting iterator" << std::endl;
	// std::cout << "graph ids: ";
	// std::cout << graph_pool.capacity() << " ";
	
	//std::cout << std::endl;
	delete(current_vertices);
	delete[](data);
	delete(bi);
}

bool CombinationIterator::next()
{
	++current_index;
	if(is_done())
		return false;

	update_data();
	return true;
}

// update the bool matrix data
// by converting the current index
// to binary
//opt: can use gray code here
//opt: current_vertices should be a doubly linked list
//opt: another way
//opt: int[num_vertices] and each entry specifies the next index
//opt: 1 -> 3 -> 7 will be stored like this
//opt: ix:   [ 0  1  2  3  4  5  6  7  8  9]
//opt: vals: [-1  3 -1  7 -1 -1 -1 -1 -1 -1]
void CombinationIterator::update_data()
{
	in_valid_vertices = 0;
	int ix_tmp = current_index*1;
	int remainder;
	int ix_element=0;
	current_vertices->reset();
	FORix(in_vertices) {
		remainder = ix_tmp % 2;
		data[ix] = (remainder==1);
		if(remainder==1) {
			++in_valid_vertices;
			current_vertices->add(ix);
			ix_element++;
		}
		ix_tmp /= 2;
	}
}

bool CombinationIterator::is_done()
{
	return (current_index >= max_index);
}

const bool* CombinationIterator::get_elements()
{
	return data;
}

BoolIterator* CombinationIterator::elements_iterator()
{
	bi->init(data, in_vertices);
	return bi;
}

VectorFixed* CombinationIterator::get_current_vertices()
{
	return current_vertices;
}

int CombinationIterator::get_valid_vertices_count()
{
	return in_valid_vertices;
}

int CombinationIterator::get_current_index()
{
	return current_index;
}

void CombinationIterator::init_current_vertices()
{
	current_vertices->reset();
}

//todo
void CombinationIterator::print()
{
	// std::cout << "ix: " << current_index << std::endl;
	//block->print("cgi");
}


