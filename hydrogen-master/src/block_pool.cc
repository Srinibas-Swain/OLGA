#include "block_pool.hh"

#include <string>


using namespace ga;

void BlockPool::init_pool(BlockPool::size_type _block_size, BlockPool::size_type in_elements)
{
	// std::cout << "bp/init-pool ..." << _block_size << std::endl;
	VPool::clear();
	FORix(number_of_threads) {
		if(!free_elements[ix].empty()) {
			std::ostringstream ss_err;
			ss_err << "bp/init: free-elements of thread " << ix << " not empty";
			ss_err << " but of size @ " << free_elements[ix].size(); 
			throw std::runtime_error(ss_err.str());
		}
	}
	block_size = _block_size;
	init_vpool(in_elements);
}

ga::Block* BlockPool::get()
{
	// DEBUG2 std::cout << "bp/get ... " << std::endl;
	ga::Block* block = (ga::Block*) get_free_element();
	// if(block->capacity() != n) {
	// 	std::ostringstream ss_err;
	// 	ss_err << "blk-pool: requested size " << n << "; got " << block->capacity() << "\n";
	// 	throw std::runtime_error(ss_err.str());
	// }
	return block;
}

BlockPool::BlockPool(BlockPool::size_type _blk_size) : block_size(_blk_size)
{
	class_name = "block-pool";
}

ga::Block* BlockPool::create_element()
{
	ga::Block* block = new ga::Block(block_size);
	//block->id = 55;
	// DEBUG3 std::cout << "bp/created " << block->class_name << "#" << block->id << "#" << std::endl;
	block->in_use = false;
	return block;
}
