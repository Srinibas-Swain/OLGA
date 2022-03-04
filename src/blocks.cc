
#include "blocks.hh"

#include <iostream>

#include "permutation_pool.hh"
#include "utils.hh"

using namespace std;
using namespace ga;

typedef typename std::vector<int>::size_type size_type;

Blocks::Blocks()
{
	class_name = "blocks";
	initialize();
}

Blocks::Blocks(size_type _capacity, size_type _sz_element)
{
	class_name = "blocks";
	ga::Block *block;
	for(size_type ix=0; ix<_capacity; ix++) {
		// pool-me-once-shame-on-you
		//block = new ga::Block(_size);
		block = BlockPool::instances[_sz_element]->get();
		blocks.push_back(block);
	}
	initialize();
}

void Blocks::initialize()
{
	id = Blocks::id_g;
	// DEBUG2 std::cout << "part#" << id << " init " << std::endl;
	Blocks::count += 1;
	Blocks::id_g  += 1;
}

// add bounds check
ga::Block* Blocks::at(size_type j) const
{
	return blocks.at(j);
}

// n = 4
// n/2 = 2
// 0 1 2 3
// 3 2 1 0

// n = 5
// n/2 = 2
// 0 1 2 3 4
// 4 3 2 1 0

void Blocks::reverse()
{
	size_type n = size();
	if(n==1)
		return;
	ga::Block *block_tmp;
	for(size_type ix=0; ix<=n/2-1; ix++) {
		block_tmp = blocks.at(ix);
		blocks.at(ix) = blocks.at(n-1-ix);
		blocks.at(n-1-ix) = block_tmp;
	}
}

ga::Block* Blocks::pop()
{
	ga::Block *block = blocks.back();
	blocks.pop_back();
	return block;
}

bool Blocks::is_discrete()
{
	return true;
}

Permutation* Blocks::to_permutation()
{
	// Permutation* perm = new Permutation(size());
	Permutation* perm = PermutationPool::instances[size()]->get();
	
	FORix(size()) {
		perm->add_mapping(ix, at(ix)->head());
	}
	return perm;
}

void Blocks::push(ga::Block *block1)
{
	blocks.push_back(block1);
}

void Blocks::add_block(ga::Block *block)
{
	//ga::Block *block_new = new ga::Block(block);
	//blocks.push_back(block_new);
	blocks.push_back(block);
}

size_type Blocks::num_blocks()
{
	return blocks.size();
}

size_type Blocks::size() const
{
	return blocks.size();
}
size_type Blocks::capacity() const
{
	return blocks.capacity();
}

void Blocks::reserve(Blocks::size_type sz_new)
{
	blocks.reserve(sz_new);
}


/// benchmark-me-once-how-many-times
void Blocks::increase_by(Blocks::size_type m)
{
	blocks.resize(size()+m);
}

Blocks::~Blocks()
{
	// DEBUG2 std::cout << "~part#" << id;
	// DEBUG2 NEWLINE;
	Blocks::count -= 1;
}


void Blocks::shift(size_type j, size_type m)
{
	size_type sz_b_original = size();
	increase_by(m);
	//cout << "sz: " << sz_b_original << " j: " << j << endl;
	for(size_type ix=sz_b_original-1; ix>=j+1; ix--) {
		assign(m+ix, blocks.at(ix));
		//cout << blocks.at(ix)->size() << " "
		//	 << blocks.at(ix+m)->size() << endl;
	}
}

void Blocks::set(size_type j, ga::Block *block, string prefix)
{
	// DEBUG2 cout << " " << prefix << "[" << j << "] = ";
	// DEBUG2 block->print("");
	
	ga::Block *block_old = blocks.at(j);
	if(block_old != NULL && (block_old->capacity() > 0)) {
		// pool-me-once-shame-on-you
		// delete(block_old);
		block_old->free_element();
	}
	blocks.at(j) = block;

	// DEBUG2 NEWLINE;
}

void Blocks::assign(size_type j, size_type _size, string prefix)
{
	// pool-me-once-shame-on-you
	set(j, BlockPool::instances[_size]->get(), prefix);
}

void Blocks::assign(size_type j, ga::Block *block, string prefix)
{
	// DEBUG2 cout << " " << prefix << "[" << j << "] = ";
	// DEBUG2 block->print("");
	
	ga::Block *block_old = blocks.at(j);
	if(block_old != NULL && (block_old->capacity() > 0)) {
		// pool-me-once-shame-on-you
		// delete(block_old);
		block_old->free_element();
	}
	// pool-me-once-shame-on-you
	blocks.at(j) = block->clone();
	//// DEBUG2 print("part:assign:");
}


void Blocks::print() const
{
	print("partition");
}

bool Blocks::valid(size_type j)
{
	return (at(j)->size() > 0);
}

void Blocks::print_clean() const
{
	for(auto it : blocks) {
		it->print();
	}
	NEWLINE;
}


void Blocks::print(string prefix) const
{
	cout << prefix << " [" << size() << "] - [ ";
	cout << "{#" << id << "#} - [ ";
	for(auto it=blocks.begin(); it != blocks.end(); it++) {
		if(*it) {
			(*it)->print("");
			cout << " ";
		}
	}
	cout << "]" << endl;
}

// pool-me-once-shame-on-you
Blocks* Blocks::clone() const
{
	Blocks *q = new Blocks();
	for(auto block : blocks) {
		q->add_block(block->clone());
	}
	return q;
}

void Blocks::append(const Blocks* other) {
	reserve(size() + other->size());
	for(int ix=0; ix<other->size(); ix++)
		add_block(other->at(ix)->clone());
}


Blocks* Blocks::copy() {
	Blocks* A1 = new Blocks();
	for(auto block : blocks) {
		A1->add_block(block);
	}
	return A1;
}

size_type Blocks::non_empty_blocks() const
{
	size_type m = 0;
	for(auto block: blocks) {
		if(block->size() > 0)
			++m;
	}
	return m;
}

size_type Blocks::index_non_unit_block() const
{
	//abstract-it-out:: enumerate with index, iterator should return a tuple
	size_type ix = 0;
	for(auto block : blocks) {
		if(block->size() > 1) {
			return ix;
		}
		++ix;
	}
	return ix;
}


// pool-me-once-shame-on-you
Blocks* Blocks::create_default(size_type n)
{
	// pool-me-once-shame-on-you
	Blocks *p = new Blocks();
	// pool-me-once-shame-on-you
	//ga::Block *block1 = new ga::Block();
	ga::Block *block1 = BlockPool::instances[n]->get();
	// DEBUG2 cout << "graph size: " << n << endl;
	block1->add_range(0, n);
	p->add_block(block1);
	return p;
}


Blocks::iterator
Blocks::begin()
{
	return Blocks::iterator(blocks.begin());
}

Blocks::iterator
Blocks::end()
{
	return Blocks::iterator(blocks.end());
}
