#include "7-graph-traversal-class.hh"

using namespace ga;

void GraphTraversal::push_stuff(int num_vertices, std::queue<Graph*>& conn_graphs)
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

void GraphTraversal::yo_mama(int num_vertices)
{
	Graph::init_upper_triangular(num_vertices+1);
	std::cout << Graph::NUM_UPPER_TRIANGULAR[num_vertices] << std::endl;
	std::queue<Graph*> conn_graphs;
	push_stuff(num_vertices, conn_graphs);
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
}

// vector<long> Graph::NUM_UPPER_TRIANGULAR = vector<long>();
//
// int main()
// {
// 	std::vector<int> signals;
// 	signals.push_back(SIGSEGV);
// 	signals.push_back(SIGABRT);
// 	backward::SignalHandling sh(signals);
// 	std::cout << std::boolalpha << "sh.loaded() == " << sh.loaded() << std::endl;
	
// 	int num_vertices = 4;
// 	for(int ix=num_vertices; ix<=num_vertices; ix++)
// 		yo_mama(num_vertices);
// 	return 0;
// }


