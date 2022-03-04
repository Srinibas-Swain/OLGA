#ifndef BOOL_ITERATOR_HH
#define BOOL_ITERATOR_HH

/****
	 USAGE:
	 bi = new BI();
	 bi.init(data) // resets the counter and data as well
	 int ix_element;
	 while(bi->not_yet_done()) {
	     ix_element = bi->next();
	 }
	 delete(bi);
****/

#include "iterator.hh"

namespace ga {
	class BoolIterator : protected Iterator {
	public:
		BoolIterator();
		//todo: BoolIterator(const bool* data);

		void init(const bool* data, int n);
		bool is_done();
		bool not_yet_done();
		void reset();
	
	private:
		int current();
		const bool* data;
	};
}

#endif /* BOOL_ITERATOR_HH */
