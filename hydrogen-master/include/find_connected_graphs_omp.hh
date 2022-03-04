#ifndef FIND_CONNECTED_GRAPHS_OMP_HH
#define FIND_CONNECTED_GRAPHS_OMP_HH

#include "find_connected_graphs.hh"

namespace ga {
	class FindConnectedGraphsOmp : public FindConnectedGraphs {
	public:
		virtual void generate(int num_vertices, MenuOption*) override;
		void form_graph(int nv, int ne, int r, int comb, Graph *graph);
	private:

	};
}

#endif /* FIND_CONNECTED_GRAPHS_OMP_HH */

