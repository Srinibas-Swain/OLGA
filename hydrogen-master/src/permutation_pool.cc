#include "permutation_pool.hh"

using namespace ga;

void PermutationPool::init_pool(size_t _permutation_size, size_t in_elements)
{
	VPool::clear();
	FORix(number_of_threads) {
		if(!free_elements[ix].empty())
			throw std::runtime_error("pe-p/init: free elements not empty");	
	}
	permutation_size = _permutation_size;
	init_vpool(in_elements);
}

void PermutationPool::validate_created_elements()
{
	// DEBUG3 std::cout << "pp/validate-created-elements ... " << std::endl;
	Permutation* element;
	size_t in_elements = size();
	for(int ix=0; ix < in_elements; ix++) {
		element = get();
		validate_element(element, permutation_size);
	}
	element->free_element();
}

Permutation* PermutationPool::get()
{
	// DEBUG2 std::cout << "pe-p/get ... " << std::endl;
	Permutation* permutation = (Permutation*) get_free_element();
	return permutation;
}

void PermutationPool::validate_element(Permutation* permutation, PermutationPool::size_type n)
{
	// DEBUG3 std::cout << "validating permutation #" << permutation->id << "#" << std::endl;
	if(permutation->size() != n) {
		std::ostringstream ss_err;
		print_stats();
		ss_err << "perm-pool: requested size " << n << "; got " << permutation->size() <<
			" current perm-size: " << permutation_size;
		throw std::runtime_error(ss_err.str());
	}
}

PermutationPool::PermutationPool(size_t _permutation_size)
	: permutation_size(_permutation_size)
{
	// std::cout << "#threads: " << number_of_threads << std::endl;
	class_name = "permutation-pool";
}

Permutation* PermutationPool::create_element()
{
	Permutation* permutation = new Permutation(permutation_size);
	//permutation->id = 55;
	// DEBUG3 std::cout << "pp/created " << permutation->class_name << "#" << permutation->id << "#" << std::endl;
	permutation->in_use = false;
	return permutation;
}
