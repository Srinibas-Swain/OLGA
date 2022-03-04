#include "generic_object.hh"
#include "v_pool.hh"
#include <iostream>

using namespace ga;

GenericObject::GenericObject()
{
	in_use = false;
}

GenericObject::~GenericObject() {
	//DEBUG3 std::cout << "~go#" << id << std::endl;
}

void GenericObject::init_element()
{
	// DEBUG3 std::cout << "go#" << id << " :init" << std::endl;
}

void GenericObject::reset_element()
{
	// DEBUG3 std::cout << "go#" << id << " :reset" << std::endl;
}

void GenericObject::free_element()
{
	// DEBUG3 std::cout << "go-free " <<  class_name <<
	//	" #" << id << "#" << std::endl;
	if((pool_ref == nullptr) || (pool_ref == NULL))
		throw std::runtime_error("pool ref is null");
	((VPool*) pool_ref)->free_element(this);
}
