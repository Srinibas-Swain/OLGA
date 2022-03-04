#include "graph_generator.hh"

//#include <iostream>

#include "ncr.hh"

using namespace ga;

GraphGenerator::GraphGenerator(int _num_vertices, Ncr* ncr, GraphGenerator::Direction _direction) {
	num_vertices = _num_vertices;
	num_edges = num_vertices*(num_vertices-1)/2;
	direction = _direction;

	r_min = 0;
	// find the max r
	// keep removing r edges as long as ne - r >= (num_vertices-1)
	// holds. now since r keeps increasing, stop when equality holds
	r_max = num_edges - num_vertices + 1;
	
	compute_num_combinations(ncr);
	allocate_graph_seeds();

	if(direction == GraphGenerator::Direction::from_kn) {
		ix_comb = -1;
		r_current = r_min;
	} else {
		ix_comb = num_combinations[r_max];
		r_current = r_max;
	}
}

void GraphGenerator::compute_num_combinations(Ncr* ncr)
{
	num_combinations = new long long[r_max+1];
	num_graphs = 0;

	for(int r=r_min; r<=r_max; r++) {
		num_combinations[r] = ncr->get(num_edges, r);
		num_graphs += num_combinations[r];
	}
}

void GraphGenerator::allocate_graph_seeds()
{
	graph_seeds = (GraphSeed**) malloc(get_number_of_threads()*sizeof(GraphSeed*));
	FORix(get_number_of_threads())
		graph_seeds[ix] = new GraphSeed(num_vertices, num_edges);
}

// valid ix_comb: [0, num_combinations[r_current])
GraphSeed* GraphGenerator::next()
{
	if(direction == GraphGenerator::Direction::from_kn)
		return next_seed();
	else
		return prev_seed();
}

// valid ix_comb: [0, num_combinations[r_current])
GraphSeed* GraphGenerator::next_seed()
{
	++ix_comb;
	if(ix_comb >= num_combinations[r_current]) {
		++r_current;
		ix_comb = 0;
	}
	return form_graph_seed();
}

// valid ix_comb: [0, num_combinations[r_current])
GraphSeed* GraphGenerator::prev_seed()
{
	--ix_comb;
	if(ix_comb<0) {
		--r_current;
		ix_comb = num_combinations[r_current]-1;
	}
	return form_graph_seed();
}

GraphSeed* GraphGenerator::form_graph_seed()
{
	GraphSeed* graph_seed = graph_seeds[omp_get_thread_num()];
	graph_seed->init(r_current, ix_comb);
	return graph_seed;
}

long long GraphGenerator::get_num_graphs() const
{
	return num_graphs;
}

GraphGenerator::~GraphGenerator() {
	FORix(get_number_of_threads())
		delete(graph_seeds[ix]);
	free(graph_seeds);
	delete[](num_combinations);
}

