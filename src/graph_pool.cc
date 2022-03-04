#include <iostream>

#include "graph_pool.hh"
#include "utils.hh"

using namespace ga;

GraphPool::GraphPool()
{
	in_vertices=0;
	cleared=true;
}

GraphPool::GraphPool(int _in_vertices) {
	in_vertices=0;
	cleared=true;
	init(_in_vertices);
}

GraphPool::~GraphPool() {
	clear();
}

Graph* GraphPool::get(int in_size)
{
	Graph* graph = graph_pool[in_size];
	//graph->fill(0);
	graph->reset_all_edges();
	return graph;
}

void GraphPool::init(int _in_vertices)
{
	if(in_vertices != _in_vertices) clear();
	in_vertices = _in_vertices;
	graph_pool = std::vector<Graph *>();
	graph_pool.reserve(in_vertices);
	FORix(2)
		graph_pool[ix] = new Graph(ix);
	
	for(int in_size=2; in_size < in_vertices; in_size++) {
		graph_pool[in_size] = new Graph(in_size);
	}
	cleared=false;
}

void GraphPool::clear() {
	if(cleared) return;
	FORix(in_vertices) {
		delete(graph_pool[ix]);
	}
	cleared=true;
}


