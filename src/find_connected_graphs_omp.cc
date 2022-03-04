#include "find_connected_graphs_omp.hh"

#include <iostream>
#include <omp.h>

using namespace ga;

Graph** allocate_graphs(int num_vertices, int num_graphs)
{
	Graph **graphs = (Graph**) malloc(num_graphs*sizeof(Graph*));
	FORix(num_graphs)
		graphs[ix] = Graph::K_n(num_vertices);
	return graphs;
}

// this is going to be a little naive
// during each "r", compute the graph matrix from scratch, starting from kn
void FindConnectedGraphsOmp::generate(int num_vertices, MenuOption* menu_option)
{
	// std::cout << "fcg-omp:" << std::endl;
	// find ne
	int ne = num_vertices*(num_vertices-1)/2;
	//std::cout << "fcg-omp: ne: " << ne;
	// find the max r
	// keep removing r edges as long as ne - r >= (num_vertices-1)
	// holds. now since r keeps increasing, stop when equality holds
	int r_max = ne - num_vertices + 1;
	//std::cout << " r-max: " << r_max << std::endl;

	// get num threads and allocate graphs
	Graph **graphs = allocate_graphs(num_vertices, get_number_of_threads());

	long num_combinations;
	Graph *graph_thread; // private to each thread
	long _n_connected_graphs = 0;

	for(int r=0; r<=r_max; r++) {
		num_combinations = ncr->get(ne, r);
		//std::cout << "fcg-omp: r " << r << "/" << r_max  << " |  num-combntns: " << num_combinations << std::endl;
		omp_set_num_threads(get_number_of_threads());
		std::cout << "..."  << num_combinations  << "..." << std::endl;
		//-# pragma omp parallel shared(graphs, num_vertices, num_combinations, ne, r, _n_connected_graphs, menu_option->certs_done) private(graph_thread)
		# pragma omp parallel shared(graphs, num_vertices, num_combinations, ne, r, _n_connected_graphs) private(graph_thread)
		{
			#pragma omp for
			for(int ix_comb=0; ix_comb<num_combinations; ix_comb++) {
				graph_thread = graphs[omp_get_thread_num()];
				form_graph(num_vertices, ne, r, ix_comb, graph_thread);
				//std::cout << "{" << graph_thread->graph_id << "}";
				if(graph_thread->connected()) {
					#pragma omp atomic
					_n_connected_graphs++;
					//-menu_option->process(graph_thread);
				}
			}
		}
	}
	n_connected_graphs = _n_connected_graphs;
	// cleaning up
	//FORix(omp_get_num_threads())
	FORix(get_number_of_threads())
		delete(graphs[ix]);
	free(graphs);
}

void FindConnectedGraphsOmp::form_graph(int num_vertices, int ne, int _r, int ix_comb, Graph *graph)
{
	long val_leftp;
	int n=ne-1, r = _r, comb = ix_comb;
	for(int ix_col=1; ix_col<num_vertices; ix_col++) {
		for(int ix_row=0; ix_row<ix_col; ix_row++) {
			graph->set_edge(ix_row, ix_col);
		}
	}

	// jump thro pascal, starting from num_vertices-Choose-r
	// for(int ix_edge=0; ix_edge<ne; ix_edge++) {
	bool b_all_done = false;
	for(int ix_col=1; ix_col<num_vertices; ix_col++) {
		for(int ix_row=0; ix_row<ix_col; ix_row++) {
			// std::cout << "||  r:" << r << " n:" << n;
			if(r<1 || n<0) {
				b_all_done = true;
				break;
			}
			// find if we are going right or left
			val_leftp = ncr->get(n, r-1);
			// std::cout << " ncr:" << val_leftp << " comb:" << comb;
			if(comb >= val_leftp) {
				// we aint choosing this edge
				// std::cout << " T";
				graph->set_edge(ix_row, ix_col);
				comb -= val_leftp;
			} else {
				// std::cout << " F";
				// we are choosing this edge
				graph->remove_edge(ix_row, ix_col);
				// go down the next secondary diagonal of pascal-triangle
				--r;
			}
			--n;
		}
		if(b_all_done) break;
	}
	graph->construct_graph_id();
	// NEWLINE;
}
