#ifndef VECTOR_FIXED_HH
#define VECTOR_FIXED_HH

#include <vector>
#include "utils.hh"

// todo: template this
// or inherit from vector
namespace ga {
class VectorFixed {
public:
	VectorFixed(int _size, int invalid=-1);
	~VectorFixed();
	void reset();
	int next();
	void add(int element);
	//void operator[](int element);
	//typedef typename std::vector<int>::iterator iterator;
	//typedef typename std::vector<int>::const_iterator const_iterator;
	int* begin();
	int* end();
	int size();
	void print();
	std::string get_string();
private:
	int invalid_filler;
	//std::vector<int> data;
	int* data;
	int in_size_valid();
	int size_total, size_valid;
	int ix_current;
};
}

#endif /* VECTOR_FIXED_HH */
