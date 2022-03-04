#ifndef GRAPH_SEED_HH
#define GRAPH_SEED_HH

#include "graph.hh"

#include "ncr.hh"

namespace ga {
	class GraphSeed {
	public:
		GraphSeed(int num_vertices, int num_edges);
		~GraphSeed();

		void init(int _r_current, int _ix_comb);
		const Graph* sprout_graph(const Ncr* ncr);
	private:
		int num_vertices, num_edges;
		int r_current;
		long long ix_comb;
		Graph* graph;

		void init_graph();
	};
}

#endif /* GRAPH_SEED_HH */

