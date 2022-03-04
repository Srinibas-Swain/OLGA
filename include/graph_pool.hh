#ifndef GRAPH_POOL_HH
#define GRAPH_POOL_HH

#include <vector>
#include "graph.hh"

namespace ga {
class GraphPool {
public:
	GraphPool();
	GraphPool(int _in_vertices);
	~GraphPool();

	void init(int _in_vertices);
	void clear();

	Graph* get(int in_size);
private:
	std::vector<Graph*> graph_pool;
	int in_vertices;
	bool cleared;
};
}

#endif /* GRAPH_POOL_HH */


