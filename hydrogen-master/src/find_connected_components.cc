#include "find_connected_components.hh"

#include <iostream>

using namespace ga;

FindConnectedComponents::FindConnectedComponents() {
	
}

FindConnectedComponents::~FindConnectedComponents() {
	
}

/// @static-method
FindConnectedComponents::Graphs FindConnectedComponents::for_graph(Graph const* graph)
{
	FindConnectedComponents::Graphs conn_components;
	std::vector<int> reachability(graph->num_vertices, 0);
	Graph* component;
	int in_reachable_nodes = 1, ix_component = 1, sz_component;
	//graph->print();
	for(int node=0; node < graph->num_vertices; node++) {
		if(reachability[node] != 0) continue;
		std::vector<int> members; members.reserve(graph->num_vertices);

		visit(graph, node, reachability, ix_component, members);

		//for(auto member : members) std::cout << member << ","; NEWLINE;

		component = new Graph(members.size());
		component->copy_edges_from(graph, members);
		//component->print();
		conn_components.push_back(component);

		ix_component++;
	}
	//for(auto rch : reachability) std::cout << rch << ","; NEWLINE;
	
	return conn_components;
}

void FindConnectedComponents::visit(Graph const* graph, int node,
									std::vector<int>& reachability,
									int ix_component, std::vector<int>& members)
{
	reachability[node] = ix_component;
	members.push_back(node);
	for(int child=0; child < graph->num_vertices; child++) {
		if(reachability[child]==0 && graph->is_edge(node, child)) {
			//std::cout << "visit(" << node << "," << child << "," << ix_component <<  ")"; NEWLINE;
			visit(graph, child, reachability, ix_component, members);
		}
	}
}


