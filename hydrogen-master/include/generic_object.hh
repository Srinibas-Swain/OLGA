#ifndef GENERIC_OBJECT_HH
#define GENERIC_OBJECT_HH

#include <cstddef>
#include <string>
// #include "v_pool.hh"

namespace ga{
class GenericObject {
public:

	virtual void init_element();
	virtual void reset_element();
	virtual void free_element();

	void* pool_ref;
	// friend class VPool;
	std::size_t id;
	bool in_use, pooled;
	std::string class_name;
	//void* pool_ref;
	
	GenericObject();
	virtual ~GenericObject();
protected:

};
}

#endif /* GENERIC_OBJECT_HH */

