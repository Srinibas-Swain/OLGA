#ifndef COMBINATION_ITERATOR_HH
#define COMBINATION_ITERATOR_HH

#include <iterator>

#include "bool_iterator.hh"

/****
	 USAGE
	 CombinationIterator ci = new CombinationIterator(n, [starting_index=0])
	 while(ci->next()) {
	   const bool* elements = ci->get_elements();
	   // check for membership by iterating and
	   // do whatever the fuck you want later
	 }
	 delete(ci);
	 
****/

#include <vector>

#include "graph.hh"
#include "vector_fixed.hh"

namespace ga {
class CombinationIterator {
public:
	CombinationIterator(int n, int ix_start=0);
	~CombinationIterator();

	void print();

	const bool* get_elements();
	bool next();
	bool is_done();

	BoolIterator* elements_iterator();
	VectorFixed* get_current_vertices();
	int get_valid_vertices_count();
	int get_current_index();
		
private:
	int in_vertices;
	int current_index;
	int in_valid_vertices;
	int max_index;
	bool *data;
	BoolIterator* bi;
	//std::vector<int> current_vertices;
	VectorFixed* current_vertices;

	void init_data();
	void update_data();
	
	void init_current_vertices();
};
}

#endif /* COMBINATION_ITERATOR_HH */
