#include "block.hh"
#include "utils.hh"
#include "block_pool.hh"

#include <iostream>

using namespace std;
using namespace ga;

ga::Block* ga::Block::create(size_type _size)
{
	return new Block(_size);	
}


ga::Block::Block(ga::Block::size_type start, ga::Block::size_type stop, ga::Block::size_type increment) : Bitmap(stop)
{
	update_id();
	add_range(start, stop, increment);
}

void ga::Block::update_id()
{
	id = ga::Block::id_g;
	// DEBUG3 cout << "block#" << id << endl;
	ga::Block::id_g += 1;
}

// ga::Block::ga::Block(ga::Block* other) : Bitmap(other->capacity())
// {
// 	update_id();
// 	bm_for {
// 		_data[ix] = other->_data[ix];
// 	}
// }

ga::Block::Block(ga::Block::size_type _size) : Bitmap(_size)
{
	update_id();
	// DEBUG2 cout << "block#" << id << endl;
}

void ga::Block::copy_to(ga::Block *other) const
{
	Bitmap::copy_to(other);
}

void ga::Block::complement(ga::Block *other) const
{
	if(_capacity != other->_capacity) {
		std::cout << "fucked";
	}
	other->_num_of_set_bits = 0;
	bm_for {
		other->_data[ix] = !_data[ix];
		if(!_data[ix])
			other->_num_of_set_bits++;
	}
}

void ga::Block::add_element(ga::Block::size_type ix_element)
{
	set(ix_element);
}

ga::Block::~Block()
{
	// DEBUG2 cout << "~block#" << id << endl;
	free(_data);
}

void ga::Block::add_range(ga::Block::size_type start, ga::Block::size_type stop, ga::Block::size_type increment)
{
	
	for(ga::Block::size_type ix = start; ix < stop; ix += increment) {
		// costly
		// set(ix);
		_data[ix] = true;
		++ _num_of_set_bits;
	}
	_head = (start < _capacity) ? start : -1;
}

void ga::Block::append(ga::Block *block)
{
	validate_capacities(block->_capacity);
	bm_for {
		// costly
		// set(element);
		if(!block->_data[ix])
			continue;
		if(block->_data[ix] && _data[ix])
			continue;
		
		_data[ix]=true;
		++_num_of_set_bits;
	}
	if(block->_head < _head)
		_head = block->_head;
}

// perf-cache-this
ga::Block::size_type ga::Block::head()
{
	//return next(-1);
	if(_head == -1)
		throw std::runtime_error("head is -1");
	return _head;
}

ga::Block::size_type ga::Block::pop_front()
{
	ga::Block::size_type element = head();
	unset(element);
	return element;
}

ga::Block* ga::Block::except(ga::Block::size_type u) const
{
	// pool-me-once-shame-on-you
	ga::Block *other = clone();
	other->unset(u);
	return other;
}

void ga::Block::remove(ga::Block::size_type element)
{
	unset(element);
}

// only this
ga::Block::size_type ga::Block::intersect(const Graph* graph, ga::Block::size_type vertex) const
{
	validate_capacities(graph->num_vertices);
	
	ga::Block::size_type intersections = 0;
	// DEBUG2 std::cout << "blk:intrsct: ";
	for(int ix=0; ix<graph->num_vertices; ix++) {
		// if ix is adjacent to vertex, N_G(vertex)
		if((graph->is_edge(vertex,ix) && _data[ix]))
			intersections++;
		}
	return intersections;
}

ga::Block* ga::Block::clone() const
{
	ga::Block* other = BlockPool::instances[capacity()]->get();
	copy_to(other);
	return other;
}

/* /// perf
 * can be done efficiently but might be little complex
 * for later
 *
 */
void ga::Block::remove(ga::Block* other)
{
	// DEBUG2 other->print("blk/rm/other: ");
	validate_capacities(other->_capacity);
	bm_for {
		if(other->_data[ix] && !this->_data[ix])
			continue;
		
		_data[ix] = false;
		--_num_of_set_bits;
	}
	update_head_from_index(0);
}
