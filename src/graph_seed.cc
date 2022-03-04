#include "graph_seed.hh"

using namespace ga;


GraphSeed::GraphSeed(int _num_vertices, int _num_edges)
{
	num_vertices = _num_vertices;
	num_edges = _num_edges;
	graph = new Graph(num_vertices);
}

void GraphSeed::init(int _r_current, int _ix_comb)
{
	r_current = _r_current;
	ix_comb = _ix_comb;
}

const Graph* GraphSeed::sprout_graph(const Ncr* ncr)
{
	init_graph();
	long val_leftp;
	int n=num_edges-1, r = r_current;
	long long comb = ix_comb;
	
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
	return graph;
}

void GraphSeed::init_graph()
{
	graph->set_all_edges();
}

GraphSeed::~GraphSeed() {
	delete(graph);
}

