#include "v_pool.hh"
#include <omp.h>

using namespace ga;

typedef typename VPool::size_type size_type;

VPool::VPool() {
	number_of_threads = get_number_of_threads();
	// std::cout << "#threads: " << number_of_threads << std::endl;

	free_elements.resize(number_of_threads);
	FORix(number_of_threads)
		free_elements.push_back(std::queue<GenericObject*>());
	in_allocations=0;
	in_frees=0;
	in_reuses=0;
}

void VPool::init_vpool(size_type in_elements)
{
	//free_elements.resize(number_of_threads);
	for(int ix_thread=0; ix_thread<number_of_threads; ix_thread++)
		create_elements(ix_thread, in_elements);
}

VPool::VPool(VPool::size_type _capacity)
{
	yo_mama = "yo mama";
	free_elements.resize(number_of_threads);
	Pool::elements.reserve(_capacity);
	in_allocations=0;
	in_frees=0;
	in_reuses=0;
}

VPool::~VPool() {
	// DEBUG3 std::cout << "~vpool" << std::endl;
	clear();
}

VPool::size_type VPool::size() const
{
	return Pool::elements.size();
}

VPool::size_type VPool::capacity() const
{
	return Pool::elements.capacity();
}


VPool::size_type VPool::size_free() const
{
	return free_elements.size();
}

int VPool::get_thread_index()
{
	return is_parallel() ? omp_get_thread_num() : 0;
}

GenericObject* VPool::get_free_element() {
	int tid = 0;

	#ifdef _USE_OPENMP
	//tid = get_thread_index();
	tid = omp_get_thread_num();
	#endif

	// std::cout << "tid: " << tid << std::endl;
	// std::cout << "fe: " << free_elements.size() << std::endl;
	//DEBUG3 std::cout << "vp/get : capacity:" << capacity() << " empty?:" << free_elements.empty() << std::endl;
	if(free_elements[tid].empty()) {
		// DEBUG2 std::cout << "vp/get : creating " << "element" << "s ... " << std::endl;
		create_elements(tid);
	}
	else {
		++in_reuses;
	}

	GenericObject* element;
	{
		element = free_elements[tid].front();
		// DEBUG3 std::cout << "vp/get : ";
		// DEBUG3 std::cout << element->class_name << " #";
		// DEBUG3 std::cout << element->id << "# in-use? " << element->in_use  << std::endl;
		free_elements[tid].pop();
		init_element(element);
	}
	return element;
}


void VPool::init_element(GenericObject* element)
{
	if(element == NULL)
		throw std::runtime_error("vp/init-element: nullptr");

	if(element->in_use) {
		std::ostringstream ss_err;
		ss_err << element->class_name << "#" << element->id << "# is in use";
		throw std::runtime_error(ss_err.str());
	}
	// DEBUG3 std::cout << "vp:init-" << element->class_name << std::endl;
	element->in_use = true;
	// DEBUG3 std::cout << "vp:init-" << element->class_name << std::endl;
	element->init_element();
}


void VPool::create_elements(int thread_index, size_type in_elements)
{
	// DEBUG2 std::cout << "vp/create-elements : in_elements: " << in_elements << std::endl;
	VPool::size_type sz_new = (in_elements>0) ? in_elements : get_size_new();
	// DEBUG2 std::cout << "vp/create-elements : creating " << sz_new << " elements ... " << std::endl;

	//Pool::elements.reserve(size()+sz_new);
	in_allocations += sz_new;

	for(int ix=0; ix<sz_new; ix++) {
		GenericObject* element = create_element();
		// DEBUG2 std::cout << "vp:created" << element->class_name <<
		//	" #" << element->id << "#" << std::endl;
		add_element(thread_index, element);
	}
}


VPool::size_type VPool::get_size_new() const
{
	VPool::size_type sz_new = size();
	if(sz_new==0)
		sz_new = 100;
	return sz_new;
}


void VPool::add_element(int thread_index, GenericObject* element)
{
	// DEBUG2 std::cout << "vp:add " << element->class_name <<
	//	" #" << element->id << "# " << std::endl;
	element->pooled = true;
	element->pool_ref = this;
	element->in_use = false;
	// dangerous
	#pragma omp critical(pool_add)
	{
		Pool::elements.push_back(element);
	}
	free_elements[thread_index].push(element);
	//GenericObject* elem = free_elements.front();
	//DEBUG2 std::cout << "vp:add-elem : added #" << elem->id << std::endl;

}

void VPool::clear()
{
	// DEBUG3 std::cout << class_name << "/vp:clear" << std::endl;
	for(int ix_thread=0; ix_thread<number_of_threads; ix_thread++) {
		while(!free_elements[ix_thread].empty()) {
			free_elements[ix_thread].pop();
		}
	}
	// DEBUG3 std::cout << "vp:cleared " << std::endl;
	for(int ix_thread=0; ix_thread<number_of_threads; ix_thread++) {
		// DEBUG3 std::cout << "capacity @ " << free_elements[ix_thread].size() << std::endl;
	}
	Pool::clear();
}

void VPool::print_stats()
{
	std::cout << "\n\n\n\n";
	std::cout << class_name << ": " <<  std::endl;
	std::cout << "size: " << elements.size() << std::endl;
	std::cout << "capacity: " << elements.capacity() << std::endl;
	std::cout << "empty: " << free_elements.empty() << std::endl;
	std::cout << "size free: " << free_elements.size() << std::endl;
	std::cout << "allocations: " << in_allocations << std::endl;
	std::cout << "reuses: " << in_reuses << std::endl;
	std::cout << "frees: " << in_frees << std::endl;
	std::cout << "deletes: " << num_deletes << std::endl;
}


void VPool::free_element(GenericObject* element)
{
	int tid=0;
	#ifdef _USE_OPENMP
	//tid = get_thread_index();
	tid = omp_get_thread_num();
	#endif

	++in_frees;
	// DEBUG2 std::cout << "vp:free-elem" << std::endl;
	if(!(element->in_use))
		throw std::runtime_error(class_name + " not in use? how come");
	element->in_use = false;
	element->reset_element();
	// have to set the tid of the element
	free_elements[tid].push(element);
}

