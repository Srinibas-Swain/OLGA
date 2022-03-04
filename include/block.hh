#ifndef __BLOCK_HH
#define __BLOCK_HH

#include "aliases.hh"
#include "bitmap.hh"
#include "graph.hh"

#include <vector>

namespace ga {
	class Block : public Bitmap {
	public:
		typedef typename std::vector<int>::size_type size_type;

		// for pooling
		static int id_g;
		friend class BlockPool;

		// features
		// dirty
		void add_range(size_type start, size_type stop, size_type increment=1);
		// dirty
		void add_element(size_type element);
		size_type intersect(const Graph* g, size_type vertex) const;
		void assign_by_copy(Block* block);
		void copy_to(Block* block) const;
		void complement(Block* other) const;
	
		std::vector<int> copy() const;
		Block* clone() const;
		void remove(Block* block);
		void remove(size_type element);
		void append(Block* block);
		size_type head();
		size_type pop_front();
		Block* except(size_type u) const;

		static Block* create(size_type _size);
	
	protected:
		Block();
		Block(Block* block);
		Block(size_type _size);
		Block(size_type start, size_type stop, size_type increment=1);
		~Block();

		void update_id();
	
	private:
	};
}


#endif
