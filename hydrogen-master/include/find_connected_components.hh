#ifndef FIND_CONNECTED_COMPONENTS_HH
#define FIND_CONNECTED_COMPONENTS_HH

#include <vector>

#include "graph.hh"

namespace ga {
class FindConnectedComponents {
public:
	typedef std::vector<Graph*> Graphs;
	FindConnectedComponents();
	~FindConnectedComponents();

	// deallocation on the part of the caller
	static Graphs for_graph(Graph const* graph);
	static void visit(Graph const* graph, int node, std::vector<int>& reachability, int ix_component, std::vector<int>&);
private:
};
}

#endif /* FIND_CONNECTED_COMPONENTS_HH */


