#ifndef V_POOL_HH
#define V_POOL_HH

#include "pool.hh"
#include <queue>

#include <iostream>

namespace ga{
class VPool : public Pool {
public:
	VPool(size_type _capacity);
	virtual void free_element(GenericObject* element);

	virtual size_type capacity() const override; 
	virtual size_type size() const override;
	virtual size_type size_free() const;
	virtual void clear() override;
	virtual void print_stats();

	VPool();
	virtual ~VPool();
protected:
	virtual GenericObject* get_free_element();
	virtual GenericObject* create_element() = 0;
	size_type get_size_new() const;
	void add_element(int thread_index, GenericObject* element);
	void init_element(GenericObject* element);
	int get_thread_index();
	void init_vpool(size_type in_elements);

	std::vector< std::queue<GenericObject*> > free_elements;
	size_type in_allocations, in_frees, in_reuses;
	std::string class_name;
	std::string yo_mama;
	int number_of_threads;
	
private:
	void create_elements(int thread_index, size_type in_elements = 0);
};
}

#endif // V_POOL_HH

