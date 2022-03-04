#ifndef __PARTITION_HH
#define __PARTITION_HH

#include "blocks.hh"
#include "graph.hh"

namespace ga {
#define partition_for for(int ix=0; ix<partition_size; ix++)
	
	class Partition : public Blocks {
	public:
		typedef typename std::vector<int>::size_type size_type;

		// for pooling
		static int id_g;
		friend class PartitionPool;
		virtual void init_element() override;

		// features
		enum PartCompare {WORSE = -1, EQUAL = 0, BETTER = +1};
		Partition* clone() const;
		static Partition* create_default(int n);
		// gives the #non-empty blocks
		virtual size_type size() const override;
		size_type compute_size() const;
		// gives the partition size
		virtual size_type capacity() const override;
		//virtual ga::Block* at(size_type j) const;
		virtual void free_element() override;
		void append_to(Blocks* other) const;
		void shift(size_type j, size_type m) override;
		//ga::Block* at(size_type j) const override;

		void initialize();

		// dangerous stuff
		virtual size_type size_cached() const;
		void update_size();
		size_type num_non_empty_elements;
		
		//
		// compare this partition with another
		// another < this  => worse
		// another > this  => better
		// another == this => equal
		Partition::PartCompare compare(const Graph* G, const Partition* mu) const;
		
	protected:
		Partition(size_type _size);
		~Partition();

	private:
		size_type partition_size;
	};
};


#endif


