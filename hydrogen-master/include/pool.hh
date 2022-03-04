#ifndef POOL_HH
#define POOL_HH

#include <vector>
#include "utils.hh"
#include "generic_object.hh"

#include <functional>
#include <stdexcept>

namespace ga{
class Pool {
public:
	typedef typename std::vector<GenericObject*>::size_type size_type;

	//using size_type = std::vector<T>::size_type;
	
	void init(size_type _sz_elements);
	void reserve(size_type _sz_elements_new);
	// dangerous, so now including it now
	// void increase_by(int _sz_elements_extra);
	virtual void clear();
	virtual size_type capacity() const; 
	virtual size_type size() const;

	GenericObject& get(size_type ix_element);
	Pool();
	virtual ~Pool();
	
protected:
	Pool(size_type _sz_elements);
	std::vector<GenericObject*> elements;
	size_type sz_elements;
	bool cleared;
	size_type num_deletes;
};
}

#endif /* POOL_HH */


