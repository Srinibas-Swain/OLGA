

* create-element()
	* shouldn't be implemented in v-pool
	* hence should be done only in the child class
* get()
	* should be (compulsorily?) be implemented in child class of v-pool
	* it should call the parent's get(). better that way i guess

This is the skeleton for a element class w/ the corresponding pool class.


protected:
	
	
private:
	// store the parameters to be passed to Element's constructor here
	long id_inc;
};

> ElementPool class
```c++
#include "v_pool.hh"
#include "element.hh"

namespace ga {
	class ElementPool : public VPool {
	public:
		static ElementPool instance;

		// this is the entry point for the pool object which takes
		// parameters to be used while calling the corresponding
		// protected constructor of the Element class.
		// since the "instance" member is static, we have to have this workaround
		void init_pool(long _block_size) {
			VPool::clear();
			block_size = _block_size;
			create_elements();
		}
		
		// this is a mandatory one, at least in most of the cases i can envision
		virtual Element* get() override {
			return (Element*) VPool::get();
		}
		
		virtual Element* get(int n) {
			Element* block = get();
			if(element->capacity() != n) {
				std::ostringstream ss_err;
				ss_err << "requested size " << n << "; got " << element->capacity() << "\n";
				throw std::runtime_error(ss_err.str());
			}
			return element;
		}
		
		
	protected:
		// Wrappers for element's protected constructors
		// the pmtrs (ex: _id_inc) which are obtained in ElementPool's constructor
		// are passed along to the corresponding constructor of the Element class.
		virtual Element* create_element() override {
			return Element(_id_inc);
		}
		
	private:
		long block_size;
	};
}
```

> Element class header
```c++
#include "generic_object.hh"

namespace ga {
	class Element : public GenericObject {
	public:
		typedef typename std::vector<int>::size_type size_type;

		// 1. for pooling
		//
		// 1.1 keeps the count
		static int id_g;
		
		// 1.2 this is mandatory
		friend class ElementPool;
		
		// 1.3 override the init and reset if necessary
		//
		// 1.3.1 called as part of get()
		// virtual void init() override; 
		//
		// 1.3.2 called as part of free_element()
		// virtual void reset() override;

		// 2. business of Element
		// features
		// ...
	
	protected:
		// Define your constructors here
		// example
		Element(int _id_inc) {
			id_g += _id_inc;
			id = id_g;
		}
		Element();
		Element(size_type _size);
		~Element();
	};
}
```

> Element class source
``` c++
#include "block.hh"
#include "block_pool.hh"
```

> usage
``` c++
// this happens in welcome.cc, outside any method/class
ElementPool ep = ElementPool();

// this is wherever you wanna use the pooled object
int some_size = 5;
Element* element = ep->get(some_size);
// use element however you want
// then free the damn thing for the benefit of the society
element->free_element();
```

