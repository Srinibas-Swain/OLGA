#ifndef BLOCK_POOL_HH
#define BLOCK_POOL_HH

#include "v_pool.hh"
#include "block.hh"

#include <iostream>

namespace ga {
	class BlockPool : public VPool {
	public:
		typedef typename std::vector<GenericObject*>::size_type size_type;
		static const size_type MIN_CAPACITY;
		
		static BlockPool instance;
		static BlockPool **instances;

		BlockPool(size_type _blk_size=0);
		virtual void init_pool(size_type _block_size, size_type in_elements);
		virtual ga::Block* get();
	protected:
		virtual ga::Block* create_element() override;
		
	private:
		long block_size;
	};
}

#endif /* BLOCK_POOL_HH */

