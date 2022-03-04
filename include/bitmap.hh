#ifndef BITMAP_HH
#define BITMAP_HH

#include <string>
#include <stdexcept>
#include <vector>

#include "generic_object.hh"


namespace ga{
	
#define bm_for for(size_type ix=0; ix<capacity(); ix++)

class Bitmap : public GenericObject {
public:
	typedef typename std::vector<int>::size_type size_type;

	virtual void init_element() override;
	virtual void reset_element() override;

	// costly for bulk
	void set(size_type i);
	void set_(size_type i);
	void set_all();
	void unset_all();
	bool is_set(size_type i) const;
	bool is_set_(size_type i) const;
	bool is_all_set() const;
	bool is_all_unset() const;
	void unset(size_type i, bool update_head=true);
	void unset_(size_type i, bool update_head=true);
	virtual void reset();

	void print(std::string prefix="") const;
	size_type size() const;
	size_type capacity() const;
	size_type num_matches(Bitmap* other) const;
	void copy_to(Bitmap* other) const;
	
	bool equal(const Bitmap* other) const;
	bool operator==(const Bitmap* other) const;
	bool subset_of(const Bitmap* other) const;
	bool operator<=(const Bitmap* other) const;
	bool proper_subset_of(const Bitmap* other) const;
	bool operator<(const Bitmap* other) const;

	// set operations
	void union_update(const Bitmap* other);
	void intersection(const Bitmap* other, Bitmap* dst);
	void intersection_update(const Bitmap* other);
	void difference_update(const Bitmap* other);

	class iterator {
		const Bitmap* _bm;
		size_type _ix;
	public:
		iterator(const Bitmap* bm);
		iterator(size_type n);
		size_type operator*();
		iterator operator++(int junk);
		iterator operator++();
		bool operator!=(const iterator& rhs);
	};
	
	size_type next(size_type ix=-1) const;
	iterator begin() const;
	iterator end() const;
protected:
	Bitmap(size_type n);
	void init_bitmap(size_type n);
	void validate_bounds(size_type ix) const;
	void validate_capacities(const Bitmap* other) const;
	void validate_capacities(size_type other_capacity) const;
	void update_head_from_index(size_type ix_start=0);
	void update_num_of_set_bits();

	// container
	bool* _data;
	// capacity of data, #bytes/bits
	size_type _capacity;
	// number of set bits
	size_type _num_of_set_bits;
	// ix of the first set bit
	size_type _head;
};
}

#endif /* BITMAP_HH */

