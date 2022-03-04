#include "graph_utils.hh"

#include "block_pool.hh"

using namespace ga;

GraphUtils::GraphUtils() {

}

GraphUtils::~GraphUtils() {

}

Blocks*
GraphUtils::max_cliques(const ga::Graph *graph, bool debug)
{
	Blocks* _max_cliques = new Blocks();
	Block* all_v = BlockPool::instances[graph->num_vertices]->get();
	all_v->set_all();

	Blocks *nbrs = neighbors(graph);
	if(debug) nbrs->print("max_cliques");
	if(debug) std::cout << std::endl;
	if(debug) graph->print2(" mc");
	if(debug) std::cout << std::endl;
	ga::Block* R = BlockPool::instances[graph->num_vertices]->get();
	ga::Block* X = BlockPool::instances[graph->num_vertices]->get();
	bron_kerbosch1(graph->num_vertices,
				   R, all_v, X,
				   _max_cliques, nbrs);

	R->free_element();
	X->free_element();
	for(ga::Block* block : *nbrs) block->free_element();
	delete(nbrs);
	all_v->free_element();
	return _max_cliques;
}

void
GraphUtils::bron_kerbosch1(int n, ga::Block *R, ga::Block *P, ga::Block *X, ga::Blocks *_max_cliques, ga::Blocks *_nbrs, int level,
						   bool debug)
{
	std::ostringstream oss;
	FORix(level) oss << "  ";
	std::string str_level = oss.str();

	if(P->is_all_unset() & X->is_all_unset()) {
		if(debug) R->print(str_level + "found clique");
		if(debug) {NEWLINE; NEWLINE;}
		ga::Block *max_clique = BlockPool::instances[n]->get();
		R->copy_to(max_clique);
		_max_cliques->add_block(max_clique);
	}
	
	
	ga::Block* P_dirty     = BlockPool::instances[n]->get();
	ga::Block* P_original  = BlockPool::instances[n]->get();
	ga::Block* X_dirty     = BlockPool::instances[n]->get();

	Block *nbrs_v;
	P->copy_to(P_original);
	
	for(int v : *P_original) {
		if(debug) R->print(str_level + " R: ");
		if(debug) X->print(" X: ");
		if(debug) P->print(" P: ");
		if(debug) std::cout << " v: " << v << std::endl;
		if(v==-1) {
			std::cout << "capacity: " << P->capacity() << " num-set-bits: " << P->size() << std::endl;
			std::cout << "begin != end ? " << (P->begin()!=P->end()) << std::endl;
		}
		nbrs_v = _nbrs->at(v);
		R->set_(v);
		P->intersection(nbrs_v, P_dirty);
		X->intersection(nbrs_v, X_dirty);

		bron_kerbosch1(n, R, P_dirty, X_dirty, _max_cliques, _nbrs, level+1);

		R->unset_(v);
		P->unset_(v);
		X->set_(v);
	}

	X_dirty->free_element();
	P_original->free_element();
	P_dirty->free_element();
}

Blocks*
GraphUtils::neighbors(const ga::Graph *graph)
{
	Blocks* _neighbors = new Blocks(graph->num_vertices, graph->num_vertices);
	for(int row=0; row<graph->num_vertices; row++) {
		ga::Block* block = _neighbors->at(row);
		block->unset_all();
		for(int col=0; col<graph->num_vertices; col++) {
			if(row == col) continue;
			
			if(graph->is_edge(row, col))
				block->set_(col);
		}
	}
	return _neighbors;
}


