#ifndef PARTITION_POOL_HH
#define PARTITION_POOL_HH

#include "v_pool.hh"
#include "partition.hh"

namespace ga {
	class PartitionPool : public VPool {
	public:
		static PartitionPool instance;
		static PartitionPool **instances;

		PartitionPool(size_type _partition_size=0);
		virtual void init_pool(size_type _partition_size, size_type in_elements);
		virtual Partition* get();
		
	protected:
		virtual Partition* create_element() override;
		
	private:
		size_type partition_size;
		void validate_element(Partition* partition, PartitionPool::size_type n);
		void validate_created_elements();
	};
}

#endif /* PARTITION_POOL_HH */

