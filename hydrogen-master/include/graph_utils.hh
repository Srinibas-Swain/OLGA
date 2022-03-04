#ifndef GRAPH_UTILS_HH
#define GRAPH_UTILS_HH

#include "blocks.hh"
#include "graph.hh"

namespace ga {
	class GraphUtils {
	public:
		GraphUtils();
		~GraphUtils();

		static Blocks* max_cliques(const Graph* graph, bool b_debug=false);
		static void bron_kerbosch1(int n, ga::Block* R, ga::Block* P, ga::Block* X, Blocks* _max_cliques,
								   Blocks* _nbrs, int level=0, bool b_debug=false);
		static Blocks* neighbors(const Graph* graph);
	private:
		
	};
}

#endif /* GRAPH_UTILS_HH */

