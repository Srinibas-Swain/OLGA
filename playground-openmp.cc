#include <functional>
#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <map>

#include "graph.hh"

#include "block_pool.hh"
#include "partition_pool.hh"
#include "permutation_pool.hh"
#include "find_connected_graphs.hh"
#include "schreiersims.hh"
#include "certificate.hh"

#define BACKWARD_HAS_BFD 1
#include "backward.hpp"

#include <queue>

using namespace std;
using namespace ga;
vector<long> Graph::NUM_UPPER_TRIANGULAR = vector<long>();

typedef typename std::vector<int>::size_type size_type;

int Block1::id_g = 0;
int Partition::id_g = 0;

size_t Permutation::id_g = 0;
size_t Permutation::count = 0;
size_type Blocks::id_g = 0;
size_type Blocks::count = 0;

const size_type BlockPool::MIN_CAPACITY = 10;

long long Schreiersims::id_g = 0;
vector<int> Schreiersims::stats_size = vector<int>(100, 0);
vector<int> Schreiersims::stats_capacity = vector<int>(100, 0);

BlockPool BlockPool::instance = BlockPool();
PartitionPool PartitionPool::instance = PartitionPool();
PermutationPool PermutationPool::instance = PermutationPool();


void push_stuff2(int num_vertices, std::queue<Graph*>& conn_graphs)
{
	Graph* graph = Graph::K_n(num_vertices);
	Graph* graph_clone;

	int tid=1;

	int ne = graph->number_of_edges();
	int nv = graph->num_vertices;
	std::cout << "graph: " << ne << ", " << nv << std::endl;
	if(nv != num_vertices)
		throw std::runtime_error("nv != num_vertices");
	
	for(int ix=0; ix<nv; ix++) {
		for(int jx=ix+1; jx<nv; jx++) {
			if(!graph->is_edge(ix,jx))
				continue;

			graph_clone = graph->clone();
			graph_clone->remove_edge(ix, jx);

			if(graph_clone->connected()) {
				conn_graphs.push(graph_clone);
			}
		}
	}
	delete(graph);
}

int main ()
{
	// for handling exceptions and printing stacktraces
	std::vector<int> signals;
	signals.push_back(SIGSEGV);
	signals.push_back(SIGABRT);
	backward::SignalHandling sh(signals);
	std::cout << std::boolalpha << "sh.loaded() == " << sh.loaded() << std::endl;

	int in_vertices = 4;
	int num_vertices = in_vertices;
	Graph::init_upper_triangular(num_vertices+1);
	std::cout << Graph::NUM_UPPER_TRIANGULAR[num_vertices] << std::endl;
	std::queue<Graph*> conn_graphs;
	push_stuff2(num_vertices, conn_graphs);
	Graph *graph_clone, *graph;
	std::cout << "size: " << conn_graphs.size() << std::endl;
	bool not_found;
	int tid;
	std::map<long long, bool> present;
	omp_set_num_threads(4);
	
	while(!conn_graphs.empty()) {
		#pragma omp parallel
		#pragma omp single
		{
			std::cout << "size: " << conn_graphs.size() << std::endl;
			for (int i = 0; i < conn_graphs.size(); i++) {
				#pragma omp task shared(conn_graphs) private(graph_clone, graph, tid, not_found)
				{
					graph = conn_graphs.front();
					conn_graphs.pop();
					if(graph != NULL) {
						#pragma omp critical(not_found_man)
						{
							not_found = (present.find(graph->graph_id) == present.end());
							present[graph->graph_id] = true;
						}
					}

					if(graph != NULL && not_found) {
						int ne = graph->number_of_edges();
						int nv = graph->num_vertices;

						tid = omp_get_thread_num();
						std::cout << "tid: " << tid << std::endl;
	
						for(int ix=0; ix<nv; ix++) {
							for(int jx=ix+1; jx<nv; jx++) {
								if(!graph->is_edge(ix,jx))
									continue;

								graph_clone = graph->clone();
								graph_clone->remove_edge(ix, jx);

								if(graph_clone->connected()) {
									#pragma omp critical
									{
										conn_graphs.push(graph_clone);
									}
								}
							}
						}
						delete(graph);
					}
				}
			}
		}
	}
	std::cout << "empty? " << conn_graphs.empty() << std::endl;
	
	return 0;
}
