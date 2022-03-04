#ifndef BLOCKS_HH
#define BLOCKS_HH

#include <vector>
#include <string>

#include "block_pool.hh"
#include "constants.hh"
#include "permutation.hh"
#include "generic_object.hh"

namespace ga {
	using namespace std;
	class Blocks : public GenericObject {
	public:
		typedef typename std::vector<int>::size_type size_type;
		
		static size_type count;
		static size_type id_g;
	
		Blocks();
		Blocks(size_type _capacity, size_type _sz_element);
		~Blocks();
		void initialize();
	
		void add_block(ga::Block* block);
		size_type num_blocks();

		// gives the vector size
		virtual size_type size() const;
		// gives the vector capacity
		virtual size_type capacity() const;

		// wrapper for underlying vector
		// idempotent if vector is
		void reserve(size_type m);
		
		// not idempotent
		void increase_by(size_type m);
		
		virtual ga::Block* at(size_type j) const;
		void set(size_type j, ga::Block* block, string prefix="");
		virtual void shift(size_type j, size_type m);
		
		void print() const;
		void print(string prefix="partition") const; 
		void print_clean() const;

		bool valid(size_type j);
		void assign(size_type j, ga::Block* block, string prefix="");
		void assign(size_type j, size_type element, string prefix="");

		void append(const Blocks* other);
		Blocks* copy();
		Blocks* clone() const;

		size_type non_empty_blocks() const;
		size_type index_non_unit_block() const;

		void push(ga::Block *block1);
		void reverse();
		ga::Block* pop();
		bool is_discrete();
		Permutation* to_permutation();

		typedef typename std::vector<ga::Block*> vec_block;
		typedef typename vec_block::iterator vec_block_iterator;
		
		class iterator : public vec_block_iterator {
		public:
			iterator(vec_block_iterator c) : vec_block_iterator(c) {}
			// ga::Block* operator*() {
			// 	return vec_block_iterator::operator *();
			// }
		};


		// class methods
		static Blocks* create_default(size_type n);


		iterator begin();
		iterator end();

		// class variables
		std::vector<ga::Block*> blocks;
		// private
	
	};
}

#endif /* BLOCKS_HH */

