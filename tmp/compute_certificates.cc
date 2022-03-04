#include <iostream>
#include <chrono>
#include <string>
#include <map>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <bitset>

#include "find_connected_graphs.hh"
#include "graph.hh"
#include "block.hh"
#include "permutation.hh"
#include "certificate.hh"
#include "utils.hh"

using namespace std;
using ns = chrono::nanoseconds;
// using get_time = chrono::steady_clock;
using get_time = chrono::system_clock;

// have to remove this
int Block1::id_g = 0;
int Block1::count = 0;
int Permutation::id_g = 0;
int Permutation::count = 0;
int Partition::id_g = 0;
int Partition::count = 0;
vector<long> Graph::NUM_UPPER_TRIANGULAR = vector<long>();

void check(Schreiersims **sch)
{
	*sch = new Schreiersims();
	(*sch)->id = 5;
	cout << sch << " " << *sch << " " << endl;
}

int main(int argv, char** argc)
{
	/** Parsing input arguments *****
		range for the number of nodes in the graph for which to
		compute the certificates.
		defaults to [3, 4]
	**/
	int ix_start = 3, ix_end = 4;
	if(argv>1) ix_start = atoi(argc[1]);
	if(argv>2) ix_end   = atoi(argc[2]);
	cout << ix_start << " to " << ix_end; NEWLINE;

	FindConnectedGraphs *fcg;
	Certificate *certificate = new Certificate();
	auto func = [&] (Graph* g) {
		cout << "graph id:: " << g->graph_id << ": ";
		cout << certificate->generate_certificate2(g);
		NEWLINE;
	};

	// long long start1;
	// int n = 5;
	// Graph *k_n = Graph::K_n(n), *graph;
	// // int n_upper_triangular = n*(n-1)/2;
	// FORix(n) {
	// 	for(int jx=ix+1; jx<n; jx++) {
	// 		graph = k_n->clone();
	// 		graph->remove_edge(ix,jx);
	// 		cout << ix << ", " << jx << ": ";
	// 		printf("%4lld ", graph->graph_id);
	// 		std::bitset<10> x(graph->graph_id); 
	// 		cout << " " << x;
	// 		cout << endl;
	// 		delete(graph);
	// 	}
	// }
	// delete(k_n);
	// return 0;

	Graph::init_upper_triangular(ix_end);
	fcg = new FindConnectedGraphs();
	for(int i = ix_start; i <= ix_end; i++) {
		auto start = get_time::now();
		// start1 = getmsofday();
		// cout << "yo " << measure<>::execution(fcg->generate(i)) << " ";
		fcg->generate(i, func);
		auto end = get_time::now();
		auto diff = end - start;
		cout << "total for " << i << " nodes: ";
		// cout << fcg->connected_graphs.size();
		cout << fcg->n_connected_graphs;
		cout << " in " << chrono::duration_cast<ns>(diff).count()/1e9  << " seconds ";
		// cout << " :: " << (getmsofday() - start1);
		NEWLINE;
	}
	
	delete(certificate);
	delete(fcg);
	
	return 0;
}
