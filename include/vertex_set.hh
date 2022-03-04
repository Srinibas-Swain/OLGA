#ifndef VERTEX_SET_HH
#define VERTEX_SET_HH

#include <string>
#include <vector>

namespace ga {
	class VertexSet {
	public:
		//#define vs_for for(size_type ix=0; ix<capacity(); ix++)
		typedef typename std::vector<int>::size_type size_type;

		VertexSet(size_type n);
		~VertexSet();

		void set(size_type ix);
		void set_(size_type ix);
		void append_(size_type ix, std::string str_prefix="");

		void unset(size_type ix);
		void unset_(size_type ix);
		
		void set_all();
		void unset_all();
		void reset();
		void update_head(size_type ix_start=0);

		size_type pop_head_();
		
		bool is_set(size_type ix) const;
		bool is_set_(size_type ix) const;
		bool has_(size_type ix) const;
		bool contains_(size_type ix) const;

		bool is_all_set() const;
		bool is_all_unset() const;
		bool is_empty() const;
		bool is_empty_() const;

		bool is_not_empty() const;
		bool is_not_empty_() const;

		size_type size() const;
		size_type size_() const;
		size_type capacity() const;
		void print() const;

		//void unset(size_type i, bool update_head=true);
		//void unset_(size_type i, bool update_head=true);
		
	private:
		void init_vertex_set(size_type n);
		void validate_bounds(size_type ix) const;
		
		size_type _capacity;
		size_type _num_of_set_bits;
		size_type _head;
		bool* _data;

	};
}

#endif /* VERTEX_SET_HH */

