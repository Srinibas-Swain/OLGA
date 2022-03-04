#include "vertex_set.hh"
using namespace ga;

#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

VertexSet::VertexSet(size_type n) {
	if(n<1)
		throw std::runtime_error("vertex-set size < 1");
	init_vertex_set(n);
}

void VertexSet::init_vertex_set(size_type n)
{
	_capacity = n;
	_num_of_set_bits = 0;
	_data = (bool*) calloc(n, sizeof(bool));
}

VertexSet::~VertexSet() {
	free(_data);
}



void VertexSet::set(size_type ix)
{
	validate_bounds(ix);
	set_(ix);
}

void VertexSet::append_(size_type ix, std::string str_prefix)
{
	//std::cout << str_prefix << "append " << ix << std::endl;
	set_(ix);
}

void VertexSet::set_(size_type ix)
{
	if(ix < _head) _head = ix;
	if(_data[ix]) return;
	_data[ix] = true;
	++_num_of_set_bits;
}

void VertexSet::unset(size_type ix)
{
	validate_bounds(ix);
	unset_(ix);
}

void VertexSet::unset_(size_type ix)
{
	if(_head == ix) {
		update_head(ix+1);
	}
	if(!_data[ix]) return;
	_data[ix] = false;
	--_num_of_set_bits;
}

void VertexSet::update_head(size_type ix_start)
{
	_head = -1;
	for(int ix=ix_start; ix<_capacity; ix++) {
		if(_data[ix]) {
			_head = ix;
			break;
		}
	}
}

void VertexSet::set_all()
{
	std::memset(_data, true, sizeof(bool) * _capacity);
	_num_of_set_bits = _capacity;
	_head = 0;
}

void VertexSet::unset_all()
{
	memset(_data, false, sizeof(bool) * _capacity);
	_num_of_set_bits = 0;
	_head = -1;
}

void VertexSet::reset()
{
	unset_all();
}

VertexSet::size_type VertexSet::pop_head_()
{
	int head = _head;
	//std::cout << "#set bits :: b4:" << _num_of_set_bits << " head:" << head;
	unset_(head);
	//std::cout <<  " af: " << _num_of_set_bits << " " << std::endl;
	return head;
}

bool VertexSet::is_set(size_type ix) const
{
	validate_bounds(ix);
	return is_set_(ix);
}

bool VertexSet::is_set_(size_type ix) const
{
	return _data[ix];
}

bool VertexSet::has_(size_type ix) const
{
	return _data[ix];
}

bool VertexSet::contains_(size_type ix) const
{
	return _data[ix];
}

bool VertexSet::is_all_set() const
{
	for(size_type ix=0; ix<_capacity; ix++)
		if(!_data[ix]) return false;
	return true;
}

bool VertexSet::is_all_unset() const
{
	for(size_type ix=0; ix<_capacity; ix++)
		if(_data[ix]) return false;
	return true;
}

bool VertexSet::is_empty() const
{
	return is_all_unset();
}

bool VertexSet::is_empty_() const
{
	return (_num_of_set_bits == 0);
}

bool VertexSet::is_not_empty_() const
{
	return (_num_of_set_bits > 0);
}

void VertexSet::validate_bounds(size_type ix) const
{
	if(ix < _capacity)
		return;

	std::ostringstream ss_err;
	ss_err << "vertexset bounds: [" << ix << "] >= " << _capacity;
	throw std::runtime_error(ss_err.str());
}

VertexSet::size_type VertexSet::size_() const
{
	return _num_of_set_bits;
}

VertexSet::size_type VertexSet::size() const
{
	int num_set = 0;
	for(int ix=0; ix<_capacity; ix++)
		num_set += (_data[ix] ? 1 : 0);
	return num_set;
}

VertexSet::size_type VertexSet::capacity() const
{
	return _capacity;
}

void VertexSet::print() const
{
	std::cout << "{";
	for(int ix=0; ix<_capacity; ix++) {
		if(_data[ix]) std::cout << ix << ",";
	}
	std::cout << "}";
}


