#ifndef VECTOR_INT_POOL_HH
#define VECTOR_INT_POOL_HH

#include <vector>

namespace ga {
class VectorIntPool {
public:
	VectorIntPool();
	VectorIntPool(int _sz_elements);
	~VectorIntPool();

	void clear();
	void init(int _sz_elements);
	std::vector<int> get(int ix_element);
	int size();
private:
	std::vector<std::vector<int> > elements;
	int sz_elements;
	bool cleared;
};
}

#endif /* VECTOR_INT_POOL_HH */


