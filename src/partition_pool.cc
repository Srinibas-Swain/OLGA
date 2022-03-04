#include "partition_pool.hh"

using namespace ga;

void PartitionPool::init_pool(PartitionPool::size_type _partition_size, PartitionPool::size_type in_elements)
{
	// DEBUG3 std::cout << "pp/init-pool ..." << std::endl;
	VPool::clear();
	FORix(number_of_threads) {
		if(!free_elements[ix].empty())
			throw std::runtime_error("pp/init: free elements not empty");	
	}
	
	partition_size = _partition_size;
	init_vpool(in_elements);
}

void PartitionPool::validate_created_elements()
{
	// DEBUG3 std::cout << "pp/validate-created-elements ... " << std::endl;
	Partition* element;
	Partition::size_type in_elements = size();
	for(int ix=0; ix < in_elements; ix++) {
		element = get();
		//validate_element(element, partition_size);
	}
	element->free_element();
}

Partition* PartitionPool::get()
{
	// DEBUG2 std::cout << "pp/get ... " << std::endl;
	Partition* partition = (Partition*) get_free_element();
	// validate_element(partition, n);
	// while(partition->blocks.size() != n) {
	// 	partition->free_element();
	// 	partition = get();
	// }
	return partition;
}

void PartitionPool::validate_element(Partition* partition, PartitionPool::size_type n)
{
	// DEBUG3 std::cout << "validating partition #" << partition->id << "#" << std::endl;
	if(partition->blocks.size() != n) {
		std::ostringstream ss_err;
		print_stats();
		ss_err << "part-pool: requested size " << n << "; got " << partition->blocks.size() <<
			" current part-size: " << partition_size  <<
			" obtained part-size: " << partition->partition_size <<
			" obtained blocks-size: " << partition->blocks.size() <<
			"\n";
		throw std::runtime_error(ss_err.str());
	}
}

PartitionPool::PartitionPool(PartitionPool::size_type _blk_size) : partition_size(_blk_size)
{
	class_name = "partition-pool";
}

Partition* PartitionPool::create_element()
{
	// DEBUG3 std::cout << "pp/create-element ..." << std::endl;
	Partition* partition = new Partition(partition_size);
	//partition->id = 55;
	// DEBUG3 std::cout << "pp/created element#" << partition->id << std::endl;
	partition->in_use = false;
	return partition;
}
