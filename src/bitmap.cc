#include "bitmap.hh"

#include <cstring>
#include <iostream>
#include <sstream>

#include "utils.hh"
//#include <string>

using namespace ga;
typedef Bitmap::size_type size_type;

Bitmap::Bitmap(size_type n) {
	if(n<1)
		throw std::runtime_error("bitmap size < 1");
	class_name = "bitmap";
	init_bitmap(n);	
}

void Bitmap::init_bitmap(size_type n)
{
	_head = -1;
	_capacity = n;
	_num_of_set_bits = 0;
	_data = (bool*) calloc(n, sizeof(bool));
}

void Bitmap::validate_bounds(size_type ix) const
{
	if(ix < _capacity)
		return;

	std::ostringstream ss_err;
	ss_err << "bmap bounds: [" << ix << "] >= " << _capacity;
	throw std::runtime_error(ss_err.str());
}

void Bitmap::set(size_type ix)
{
	validate_bounds(ix);
	set_(ix);
}

void Bitmap::set_(size_type ix)
{
	if(_data[ix])
		return;
	
	if(ix < _head)
		_head = ix;
	++_num_of_set_bits;
	_data[ix] = true;
}

void
Bitmap::set_all()
{
	bm_for {
		_data[ix] = true;
	}
	_num_of_set_bits = _capacity;
	_head = 0;
}

void
Bitmap::unset_all()
{
	reset();
}

bool Bitmap::is_set(size_type ix) const
{
	validate_bounds(ix);
	return _data[ix];
}

bool Bitmap::is_set_(size_type ix) const
{
	return _data[ix];
}

bool
Bitmap::is_all_set() const
{
	bm_for {
		if(!_data[ix]) return false;
	}
	return true;
}

bool
Bitmap::is_all_unset() const
{
	bm_for {
		if(_data[ix]) return false;
	}
	return true;
}

void
Bitmap::intersection(const ga::Bitmap *other, ga::Bitmap *dst)
{
	bool in_both;
	bm_for {
		in_both = this->_data[ix] & other->_data[ix];
		dst->_data[ix] = in_both;
		dst->_num_of_set_bits += (in_both ? 1 : 0);
	}
	dst->update_head_from_index(0);
}

void
Bitmap::union_update(const ga::Bitmap *other)
{
	bm_for {
		if(other->_data[ix])
			this->set_(ix);
	}
}

void
Bitmap::difference_update(const ga::Bitmap *other)
{
	bm_for {
		if(other->_data[ix])
			this->unset_(ix, false);
	}
	this->update_head_from_index(0);
}

void
Bitmap::intersection_update(const ga::Bitmap *other)
{
	bm_for {
		// if ix is not present in both, unset
		if(!(this->_data[ix] & other->_data[ix]))
			this->unset_(ix, false);
	}
	this->update_head_from_index(0);
}

void Bitmap::unset(size_type ix, bool b_update_head)
{
	validate_bounds(ix);
	unset_(ix, b_update_head);
}

void
Bitmap::unset_(size_type ix, bool b_update_head)
{
	if(!_data[ix])
		return;

	_data[ix] = false;
	--_num_of_set_bits;
	// std::cout << " set-bits: " << _num_of_set_bits << std::endl;
	if(_head == ix && b_update_head)
		update_head_from_index(ix);
}

void Bitmap::init_element()
{
	// DEBUG3 std::cout << "bmap/init-element" << std::endl;
	reset();
}
void Bitmap::reset_element()
{
	// DEBUG3 std::cout << "bmap/reset-element" << std::endl;
	reset();
}

void Bitmap::reset()
{
	// DEBUG3 std::cout << "bmap/reset " << std::endl;
	memset(_data, false, sizeof(bool) * _capacity);
	_head = -1;
	_num_of_set_bits = 0;
}

void Bitmap::update_head_from_index(size_type ix_start)
{
	for(size_type ix = ix_start; ix<capacity(); ix++) {
		if(is_set(ix)) {
			_head = ix; return;
		}
	}
	_head = -1;
}

void Bitmap::print(std::string _prefix) const
{
	std::cout << _prefix << "{";
	bm_for {
		if(_data[ix]) std::cout << ix+1 << ", ";
	}
	std::cout << "}";
}

/// perf-cache-this
size_type Bitmap::size() const
{
	return _num_of_set_bits;
}

size_type Bitmap::capacity() const
{
	return _capacity;
}

inline void Bitmap::validate_capacities(size_type other_capacity) const
{
	if(_capacity == other_capacity)
		return;

	std::ostringstream ss_err;
	ss_err << "bmap capacities: " <<
		"  this: " <<  _capacity  <<
		" other: " << other_capacity;
	throw std::runtime_error(ss_err.str());
}

size_type Bitmap::num_matches(Bitmap* other) const
{
	validate_capacities(other->_capacity);
	// if you use bit manipulation, then this is just bit_count(a & b)!!
	size_type in_matches=0;
	bm_for {
		if(_data[ix] == other->_data[ix])
			++in_matches;
	}
	return in_matches;
}

void Bitmap::copy_to(Bitmap* other) const
{
	validate_capacities(other->_capacity);
	/// perf- memcpy
	bm_for {
		other->_data[ix] = _data[ix];
	}
	other->_head = this->_head;
	other->_num_of_set_bits = this->_num_of_set_bits;
}

bool Bitmap::equal(const Bitmap* other) const
{
	validate_capacities(other->_capacity);
	bm_for {
		if(_data[ix] != other->_data[ix])
			return false;
	}
	return true;
}

bool Bitmap::operator==(const Bitmap* other) const
{
	return equal(other);
}

bool Bitmap::subset_of(const Bitmap* other) const
{
	validate_capacities(other->_capacity);
	bm_for {
		// this is set and other is not
		if(_data[ix] && !other->_data[ix])
			return false;
	}
	return true;
}
bool Bitmap::operator<=(const Bitmap* other) const
{
	return subset_of(other);
}

/* 1. if other has none set, then this aint a proper subset 
 * 2. if this has none set,  then this is   a proper subset
 * 3. if this has a bit set and other doesnt, this aint a proper subset
 * 4. if other has a single bit not present in this,
 * then that is possible proper superset
 */
bool Bitmap::proper_subset_of(const Bitmap* other) const
{
	validate_capacities(other->_capacity);
	// cond.1
	if(other->_num_of_set_bits == 0)
		return false;
	// cond.2
	else if(this->_num_of_set_bits == 0)
		return true;
	
	bool b_superset = false;
	bm_for {
		// cond.3
		if(_data[ix] && !other->_data[ix])
			return false;
		// cond.4
		if(!b_superset && (!_data[ix] && other->_data[ix]))
			b_superset = true;
	}
	return b_superset;
}
bool Bitmap::operator<(const Bitmap* other) const
{
	return proper_subset_of(other);
}

// the next set bit after ix
// return capacity() if none present
size_type Bitmap::next(size_type ix_start) const
{
	for(size_type ix=ix_start+1; ix<capacity(); ix++) {
		if(_data[ix]) return ix;
	}
	return capacity();
}

Bitmap::iterator Bitmap::begin() const
{
	return Bitmap::iterator(this);
}

Bitmap::iterator Bitmap::end() const
{
	return Bitmap::iterator((_num_of_set_bits == 0) ? _head : capacity());
}

Bitmap::iterator::iterator(const Bitmap* bm)
{
	_bm = bm;
	_ix = bm->_head;
}

Bitmap::iterator::iterator(size_type n)
{
	_ix=n;
}

size_type Bitmap::iterator::operator*()
{
	return _ix;
}

Bitmap::iterator Bitmap::iterator::operator++(int junk)
{
	_ix = _bm->next(_ix); return *this;
}

Bitmap::iterator Bitmap::iterator::operator++()
{
	return operator++(0);
}

bool Bitmap::iterator::operator!=(const Bitmap::iterator& rhs)
{
	return _ix!=rhs._ix;
}


