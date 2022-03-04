
#include "partition.hh"
#include "partition_pool.hh"
#include "utils.hh"

#include <iostream>

using namespace ga;

typedef typename std::vector<int>::size_type size_type;

Partition::Partition(size_type _size)
	: Blocks(_size, _size)
{
	class_name = "partition";
	partition_size = _size;
	id = Partition::id_g;
	Partition::id_g += 1;
}

Partition::~Partition()
{
	// DEBUG3 std::cout << "free part#" << id << "#" << std::endl;
	// DEBUG3 std::cout << " size: " << blocks.size() << std::endl;
}

void Partition::free_element()
{
	// DEBUG2 std::cout << "free part#" << id << "#" << std::endl;
	// // DEBUG3 std::cout << " size: " << blocks.size() << std::endl;
	Blocks::free_element();
}

// pool-me-once-shame-on-you
Partition* Partition::clone() const
{
	// DEBUG3 print("part/clone/self");
	// DEBUG3 std::cout << "part-size: " << partition_size << endl;
	Partition *q = PartitionPool::instances[partition_size]->get();
	// DEBUG2 q->print("part/clone/q");
	// DEBUG3 NEWLINE;
	
	if(q->blocks.size() != blocks.size()) {
		std::ostringstream ss_err;
		ss_err << "part/clone: size mismatch"  <<
			"|this| = " << blocks.size() <<
			" |q #" << q->id <<"# | = " << q->blocks.size();
		std::string str_err = ss_err.str();
		throw std::runtime_error(str_err);
	}

	// copy each block over to the other dude
	for(int ix=0; ix < blocks.size(); ix++) {
		blocks[ix]->copy_to(q->at(ix));
	}

	// cache non-empty-elements
	// q->update_size();
	q->num_non_empty_elements = num_non_empty_elements;

	return q;
}

size_type Partition::size() const
{
	return compute_size();
}

size_type Partition::size_cached() const
{
	return num_non_empty_elements;
}

void Partition::update_size()
{
	num_non_empty_elements = compute_size();
}


void Partition::append_to(Blocks* other) const
{
	Partition::size_type sz_current = size_cached();
	other->reserve(other->size() + sz_current);

	for(int ix=0; ix<sz_current; ix++)
		other->add_block(at(ix)->clone());
}

/// perf-cache-this
size_type Partition::compute_size() const
{
	size_type in_non_empty_elements = 0;
	for(auto block : blocks) {
		if(block->size() > 0)
			++in_non_empty_elements;
	}
	return in_non_empty_elements;
}

void Partition::shift(size_type j, size_type m)
{
	size_type sz_b_original = size_cached();
	
	for(size_type ix=sz_b_original-1; ix>=j+1; ix--) {
		assign(m+ix, blocks.at(ix));
	}
}

size_type Partition::capacity() const
{
	return partition_size;
}

void Partition::init_element()
{
	// DEBUG3 std::cout << "init " << std::endl;
	// DEBUG3 std::cout << class_name << "/init " << id << std::endl;
	partition_for {
		blocks[ix]->reset();
	}
	num_non_empty_elements = 0;
}

// compare the first "l" indices of the partition
// where l is the index of the first non-unit block
Partition::PartCompare Partition::compare(const Graph *G, const Partition *mu) const
{
	int l = index_non_unit_block();
	bool mu_ij, pi_ij;
	for(int j = 0; j < l; j++) {
		for(int i = 0; i < j; i++) {
			mu_ij = G->is_edge(mu->at(i)->head(), mu->at(j)->head());
			pi_ij = G->is_edge(at(i)->head(),     at(j)->head());
			//if(mu_ij < pi_ij)
			if(!mu_ij && pi_ij)
				return WORSE;
			//else if (mu_ij > pi_ij)
			else if (mu_ij && !pi_ij)
				return BETTER;
		}
	}
	return EQUAL;
}

// Partition* Partition::at(int j) const
// {
// 	return (Partition*) Blocks::at(j);
// }


// pool-me-once-shame-on-you
Partition* Partition::create_default(int n)
{
	// pool-me-once-shame-on-you
	// Partition *p = new Partition();
	Partition *p = PartitionPool::instances[n]->get();
	// DEBUG3 p->print("part/default");
	
	// pool-me-once-shame-on-you
	//ga::Block *block1 = new ga::Block();
	// ga::Block *block1 = BlockPool::instances[n]->get();

	// DEBUG2 cout << "graph size: " << n << endl;
	ga::Block *block1 = p->at(0);
	block1->add_range(0, n);

	// cache non-empty-elements
	p->num_non_empty_elements = 1;

	return p;
}
