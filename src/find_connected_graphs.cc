#include "find_connected_graphs.hh"

#include <chrono>
#include <iostream>
#include <omp.h>
#include <sstream>
#include <sys/time.h>
#include <time.h>

#include "block.hh"
#include "find_connected_graphs_omp.hh"
#include "utils.hh"

using namespace ga;

FindConnectedGraphs::FindConnectedGraphs()
{
	ncr = new Ncr();
}

FindConnectedGraphs::FindConnectedGraphs(int n, MenuOption* menu_option)
	: FindConnectedGraphs()
{
	Graph* k_n = Graph::K_n(n);
	vector<Graph*> conn_graphs;
	generate(k_n, conn_graphs, menu_option);
	connected_graphs = conn_graphs;
}

FindConnectedGraphs::~FindConnectedGraphs()
{
	for(auto graph : connected_graphs)
		delete(graph);
	delete(ncr);
}

void FindConnectedGraphs::print()
{
	for(Graph* graph : connected_graphs) {
		graph->print();
		NEWLINE;
	}
}

// start with the completely connected graph and iteratively remove
// the edges to traverse all the possible connected graphs of size
// num_vertices.
void FindConnectedGraphs::generate(int n, MenuOption* menu_option)
{
	Graph* k_n = Graph::K_n(n);
	vector<Graph*> conn_graphs;
	n_connected_graphs = 0;
	generate(k_n, conn_graphs, menu_option);
	connected_graphs = conn_graphs;
	delete(k_n);
}


/**
   start with a graph and process it if it passes the basic checks (1
   and 2 below)
   #
   post processing:
   then, find the inferior graphs (graph which has exactly one less edge)
   iteratively by removing an edge, processing the inferior graph and
   then adding back the edge which was removed (see bottlenecks).
   #
   optimizations in that order:
   1. return if the graph is already processed
   2. return if the graph is not connected
   3. dont post-process if the graph is a tree
   since the graph is connected, check if |edges|==|nodes-1|
   #
   bottlenecks for parallelization
   1. adding the current graph to the hash/map/set has to be
   single-threaded/mutexed
   2. during post-processing (while generating inferior graphs), we
   cannot use a single graph, have to use multiple graphs.
**/
void FindConnectedGraphs::generate(Graph *graph, graphs& conn_graphs, MenuOption* menu_option)
{
	/// 1. avoid duplicate processing
	/// todo: instead of using a hash/map, can use a set, which should be way better
	if(present.find(graph->graph_id) != present.end()) {
		//delete(graph);
		return;
	}
	present[graph->graph_id] = true;

	/// 2. avoid unconnected graph
	if(!graph->connected()) {
		// delete(graph);
		return;
	}

	if(menu_option != NULL) {
		//menu_option->process(graph);
	}
	// conn_graphs.push_back(graph);
	n_connected_graphs++;

	/// 3. tree check
	int ne, nv;
	ne = graph->number_of_edges();
	nv = graph->num_vertices;
	if(ne == (nv-1)) {
		//delete(graph);
		return;
	}

	/// hack to traverse all the inferior graphs
	// Graph* graph_new;
	FORix(nv) {
		for(int jx=ix+1; jx<nv; jx++) {
			if(!graph->is_edge(ix,jx))
				continue;

			// graph_new = graph->clone();
			// graph_new->remove_edge(ix, jx);
			graph->remove_edge(ix, jx);
			generate(graph, conn_graphs, menu_option);
			graph->add_edge(ix, jx);
		}
	}
	//delete(graph);
}




using ns = chrono::nanoseconds;
using get_time = chrono::system_clock;

void FindConnectedGraphs::generate_for_all(int ix_start, int ix_end,
										  MenuOption* menu_option)
{
	init_before_big_bang(ix_start, ix_end);

	// have to take ownership of this pointer
	for(int ix = ix_start; ix <= ix_end; ix++) {
		n_connected_graphs = 0;
		auto start = get_time::now();

		menu_option->init_for_new_n(ix);
		//func_sz(ix);
		generate(ix, menu_option);

		auto end = get_time::now();
		auto diff = end - start;
		cout << "total for " << ix << " nodes: ";
		cout << n_connected_graphs << " graphs ";
		cout << "in " << chrono::duration_cast<ns>(diff).count()/1e9  << " seconds ";
		NEWLINE;
	}
}

void FindConnectedGraphs::init_before_big_bang(int ix_start, int ix_end)
{
	Graph::init_upper_triangular(ix_end);
	ncr->init(ix_end*(ix_end-1)/2);
}


void FindConnectedGraphs::for_all_graphs(int ix_start, int ix_end,
										 MenuOption* menu_option)
{
	FindConnectedGraphs *fcg = get_fcg();
	fcg->generate_for_all(ix_start, ix_end, menu_option);

	delete(fcg);
}
FindConnectedGraphs* FindConnectedGraphs::get_fcg()
{
	std::cout << "is parallel: " << is_parallel() << std::endl;
	if(is_parallel())
		return new FindConnectedGraphsOmp();
	else
		return new FindConnectedGraphs();
}

void FindConnectedGraphs::generate_parallel(int num_vertices, const std::function<void(Graph* graph)> &f)
{

	std::vector<Graph*> conn_graphs;
	Graph *graph_clone, *graph;
	bool not_found = true;
	int tid;
	std::map<long long, bool> graph_id_present;
	std::vector<bool> duplicates;

	Graph::init_upper_triangular(num_vertices+1);
	//conn_graphs.push(Graph::K_n(num_vertices));
	conn_graphs.push_back(Graph::K_n(num_vertices));
	duplicates.push_back(false);

	// std::cout << "size: " << conn_graphs.size() <<
	// 	" #threads: " << get_number_of_threads() << std::endl;
	omp_set_num_threads(get_number_of_threads());
	// omp_set_num_threads(2);
	int in_deletes=0, in_clones=0, in_processed=0;
	int sz_so_far=0, sz_new=1;
	std::ostringstream oss;
	graph_id_present[conn_graphs[0]->graph_id] = true;
	Graph *graph_validate;

	//while(!conn_graphs.empty()) {
	while((sz_new-sz_so_far) >= 1) {
		// std::cout << "size: " << conn_graphs.size() << std::endl;
		// std::cout << "sz_so_far: " << sz_so_far << ", sz_new: " << sz_new << std::endl << std::endl << std::endl;
		#pragma omp parallel
		#pragma omp single
		{

			for (int i = 0; i < sz_new-sz_so_far; i++) {
				#pragma omp task shared(conn_graphs, sz_so_far) private(graph_clone, graph, tid, not_found, oss)
				{
					graph = duplicates[i+sz_so_far] ? NULL : conn_graphs[i+sz_so_far];
					if(graph != NULL) {
						// oss << "i: " << i << " i': " << i+sz_so_far;
						// oss << " graph_id: " << graph->graph_id << std::endl;
						// std::cout << oss.str();

						f(graph);
						#pragma omp atomic
						++in_processed;

						int nv = graph->num_vertices;

						tid = omp_get_thread_num();
						// std::cout << "tid: " << tid << std::endl;

						for(int ix=0; ix<nv; ix++) {
							for(int jx=ix+1; jx<nv; jx++) {
								if(!graph->is_edge(ix,jx))
									continue;

								graph_clone = graph->clone();
								graph_clone->remove_edge(ix, jx);

								if(graph_clone->connected()) {
									#pragma omp critical(fcg_push_conn_graphs)
									{
										++in_clones;
										conn_graphs.push_back(graph_clone);
									}
								}
							}
						}
						delete(graph);
						#pragma omp atomic
						++in_deletes;
					}
				}
			}
		}
		sz_so_far = sz_new;
		sz_new = conn_graphs.size();
		duplicates.reserve(sz_new);
		// std::cout << "cleaning up ... " << std::endl;
		// for(int ix=sz_new-1; ix>=sz_so_far; ix--) {
		for(int ix=sz_so_far; ix<sz_new; ix++) {
			graph_validate = conn_graphs[ix];
			// std::cout << "ix: " << ix << " graph_id: " << graph_validate->graph_id << std::endl;
			if(graph_id_present.find(graph_validate->graph_id) == graph_id_present.end()) {
				graph_id_present[graph_validate->graph_id] = true;
				duplicates[ix] = false;
			} else {
				//conn_graphs.erase(conn_graphs.begin()+ix);
				duplicates[ix] = true;
			}
		}
		sz_new = conn_graphs.size();
	}
	std::cout <<
		"empty? " << conn_graphs.empty() <<
		" #clones: " << in_clones <<
		" #deletes: " << in_deletes <<
		" #processed: " << in_processed <<
		std::endl;
}

