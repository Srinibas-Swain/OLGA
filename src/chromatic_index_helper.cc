#include "chromatic_index_helper.hh"

#include  <iomanip>

#include "graph_utils.hh"

using namespace ga;

ChromaticIndexHelper::ChromaticIndexHelper(ChromaticIndex* _ci_mo) {
	graph_pool = new GraphPool(); // one per thread
	vi_pool = new VectorIntPool(); // one per thread
	ci_mo = _ci_mo;
	i_verbosity = 0;
	ci_debug = false;
}



ChromaticIndexHelper::~ChromaticIndexHelper() {
	delete(graph_pool); // one per thread
	delete(vi_pool); // one per thread
}

void ChromaticIndexHelper::init(int n)
{
	graph_pool->init(n+1);
	vi_pool->init(n+1);
}

// ci helper
void ChromaticIndexHelper::process_graph(const Graph* graph)
{
	//long long cert_graph = generate_certificate(graph);

	auto result_degrees = get_degrees(graph);
	int p_max_degree = result_degrees.first.first;
	bool b_single_max_degree = result_degrees.first.second;
	std::vector<int> degrees = result_degrees.second;

	long long cert_graph = ci_mo->get_certificate(graph);
	str_trash = "";

	Graph* graph_child = graph_pool->get(graph->num_vertices); // graph_pool of ci helper
	graph->copy(graph_child);
	std::ostringstream oss_out;
	// oss_out << "[x" << graph->num_vertices << "]";
	// std::cout << "[# " << std::setw(6) << graph->graph_id << "]";
	// oss_out << "[# " << std::setw(4) << cert_graph <<  "] ";
	if(i_verbosity==1) {
		std::cout << "[md " << p_max_degree <<  "] ";
		NEWLINE;
		graph->print();
	}
	//oss_out << "{smd: " << (b_single_max_degree ? "s" : " ") << "}";
	if(i_verbosity==1) NEWLINE;
	
	int ci_yo = -1;
	if(ci_mo->db_ci.find(graph->num_vertices) != ci_mo->db_ci.end() &&
	   ci_mo->db_ci[graph->num_vertices].find(cert_graph) != ci_mo->db_ci[graph->num_vertices].end()) {
		ci_yo = ci_mo->db_ci[graph->num_vertices][cert_graph];
	}
	
	//
	int ci_lb_max = -1, ci_ub_min = graph->num_vertices+2;
	// std::cout << "parent : max-deg:" << p_max_degree; NEWLINE;
	//
	// b_single_max_degree = false;
	if(graph->is_k_n(cert_graph)) {
		ci_lb_max = ci_ub_min = ((graph->num_vertices % 2) == 0) ? (graph->num_vertices-1) : graph->num_vertices;
	}
	else if(b_single_max_degree) {
		ci_lb_max = ci_ub_min = p_max_degree;
	}
	else {
		FORix(graph->num_vertices) {
			graph_child->reset_vertex(ix);

			std::pair<int, int> result = process_removed_node(graph, graph_child, ix, p_max_degree, degrees);
			if((result.first !=0) && (result.second!=0)) {
				if(result.first  > ci_lb_max)	ci_lb_max = result.first;
				if(result.second < ci_ub_min)	ci_ub_min = result.second;
			}

			graph_child->copy_edges_from(graph, ix);
		}
	}

	long long trash, ci_graph;
	bool b_lb_eq_ub = false, b_vizing_violated = false;
	b_lb_eq_ub = (ci_lb_max == ci_ub_min);
	
	if(ci_lb_max == ci_ub_min) {
		#pragma omp critical(save_parameters)
		{
			ci_mo->db_ci[graph->num_vertices][cert_graph] = ci_graph = ci_lb_max;
		}
	} else {
		// db_ci[graph->num_vertices][cert_graph] = max(ci_lb_max, ci_ub_min);
		// std::cout << "   ";

		trash = max(ci_lb_max, ci_ub_min);
		b_vizing_violated = ((trash != p_max_degree) && (trash != (p_max_degree+1)));
		ci_graph = (trash > (p_max_degree+1)) ? p_max_degree+1 : trash;
		
		#pragma omp critical(save_parameters)
		{
			++ci_mo->in_anomalies;
			ci_mo->db_ci[graph->num_vertices][cert_graph] = ci_graph;	
		}
	}
	oss_out << "[" << ci_lb_max << "," << ci_ub_min << "]{" << ci_graph << "} -" << p_max_degree << "- ";
	oss_out << (b_single_max_degree ? "smd " : "    ");
	if(b_lb_eq_ub) {
		oss_out << "          ";
	} else {
		oss_out << " lb!=ub " << (b_vizing_violated ? " vi" : "   ");
	}
	
	if((ci_graph!=p_max_degree) && ci_graph!=(p_max_degree+1)) {
		// std::cout << " Vizing's theorem violation ";
	}
	//std::cout << str_trash;

	if((ci_yo != -1) && (ci_yo != ci_graph)) {
		oss_out << "same cert, difft ci";
	}
	//oss_out << std::endl;
	#pragma omp critical(save_parameters_output)
	{
		ci_mo->db_ci_out[graph->num_vertices][cert_graph] = oss_out.str();
	}
	//NEWLINE;
}

// ci helper
// perf: not very good, for every graph, degrees get all and deall
std::pair<std::pair<int, bool>, std::vector<int> > ChromaticIndexHelper::get_degrees(Graph const* graph)
{
	std::vector<int> degrees = graph->get_degrees();
	int p_max_degree = graph->get_max_degree(degrees);
	int in_nodes_max_degree = 0;
	for(auto degree : degrees) {
		if(degree==p_max_degree) ++in_nodes_max_degree;
	}
	return std::make_pair(std::make_pair(p_max_degree, in_nodes_max_degree==1), degrees);
}

std::pair<int, int>
ChromaticIndexHelper::process_removed_node(const Graph* graph_parent,
										   Graph* graph_child,
										   int ix_node,
										   int p_max_degree,
										   std::vector<int> degrees)
{
	if(i_verbosity == 1) {
		std::cout << "rm: " << ix_node << " ";
	}
	int c_max_degree = graph_child->get_max_degree();
	// this would always access and find a legit value from db_ci?
	// if so, dont have call find_chromatic_index
	// but, the "new graph" graph_child need not be connected and we have to find
	// its connected components, which would essentially be in db_ci.
	// so should we have a seperate function instead of this line?
	int c_ci = find_chromatic_index(graph_child);

	if(i_verbosity == 1) {
		std::cout << "[md " << c_max_degree << "]";
		std::cout << "[c " << c_ci << "]";
	}

	int ci_hyp = -1;
	if(c_max_degree == p_max_degree) {
		ci_hyp = c_ci + 1;
	}
	else {
		// if node is adj to a max degree node
		if((degrees[ix_node]==p_max_degree) || has_max_degree_nbr(graph_parent, ix_node, p_max_degree, degrees)) {
			ci_hyp = c_ci + 2;
		}
		else {
			std::cerr << "node not max degree nor max degree nor adj to node with max degree";
			throw ChromaticIndex::CINodeAnomaly("node not max degree nor max degree nor adj to node with max degree");
		}
	}
	if(i_verbosity==1) {
		std::cout << "[c-hyp " << ci_hyp << "]";
		NEWLINE;
	}
	return std::pair<int, int>(c_ci, ci_hyp);
}


int ChromaticIndexHelper::find_chromatic_index(Graph const* graph)
{
	return find_max_ci(FindConnectedComponents::for_graph(graph));
}


int ChromaticIndexHelper::find_max_ci(std::vector<Graph*> conn_components)
{
	if(i_verbosity==1) {
		std::cout << "[cc ";
		std::cout << "#" << conn_components.size();
	}
	long long ci_max = 0, ci_curr, comp_certificate;
	str_trash += "{";
		for(auto component : conn_components) {
			if(component->num_vertices<=1) continue;
			// this is bad, to compute the certificate of the connected graphs
			// of "less" vertices which we have seen already.
			// have a certificate by graph_id cache
			// this is done @ compute_graph_id_and_certify()->generate_certificate()
			comp_certificate = ci_mo->get_certificate(component);

			if(i_verbosity==1) {
				std::cout << "{";
					std::cout << " ct " << comp_certificate;
				std::cout << " ci " << ((ChromaticIndex*)ci_mo)->db_ci[component->num_vertices][comp_certificate];
				std::cout << "}";
			}

			ci_curr = ((ChromaticIndex*)ci_mo)->db_ci[component->num_vertices][comp_certificate];

			str_trash += std::to_string(ci_curr);
			str_trash += ",";
		
			if(ci_curr > ci_max)
				ci_max = ci_curr;
		}
	str_trash += "}";
		for(auto component : conn_components) delete(component);
	if(i_verbosity==1)
		std::cout << " ]";
	return ci_max;
}



bool
ChromaticIndexHelper::has_max_degree_nbr(const Graph* graph,
										 int node,
										 int p_max_degree,
										 std::vector<int> degrees)
{
	for(int nbr=0; nbr<graph->num_vertices; nbr++) {
		if(is_nbr(graph, node, nbr) && (degrees[nbr]==p_max_degree)) {
			return true;
		}
	}
	return false;
}

bool
ChromaticIndexHelper::is_nbr(const Graph* graph, int node1, int node2)
{
	return graph->is_edge(node1, node2) ||
		graph->is_edge(node2, node1);
}

std::string form_header(int n)
{
	std::ostringstream oss_out;
	oss_out << "[" << n << "] ";
	FORix(n) oss_out << "  ";
	return oss_out.str();
}

int
ChromaticIndexHelper::find_exact_chromatic_index(const ga::Graph *graph)
{
	int n = 0;
	int g = graph->num_vertices;
	// both of these can be preallocated
	//Block* T = ga::Block::create(graph->num_vertices);
	Block* T = BlockPool::instances[graph->num_vertices]->get();
	Blocks* B = new Blocks();
	vertices_t_complement = new VectorFixed(graph->num_vertices, -1);
	hist_S = new int[graph->num_vertices];

	if(ci_debug) {std::cout << "start: exact ci helper ... "; NEWLINE;}
	exact_ci_helper(graph, n, g, T, B, graph->num_vertices);
	if(ci_debug) {std::cout << "done: exact ci helper ... "; NEWLINE;}

	delete[](hist_S);
	delete(vertices_t_complement);
	delete(B);
	T->free_element();
	//BlockPool::instances[graph->num_vertices]->free_element(T);
	return g;
}

void print_t_comp(int* map_for_exact_ci, int num_vertices, std::string str_header)
{
	std::cout << str_header << "T': ";
		FORix(num_vertices) {
		if(map_for_exact_ci[ix] == -1) break;
		std::cout << map_for_exact_ci[ix] << " ";
	}
	NEWLINE;
}

int* init_map(int num_vertices)
{
	int *map_for_exact_ci = new int[num_vertices];
	FORix(num_vertices) map_for_exact_ci[ix] = -1;
	return map_for_exact_ci;
}

void
ChromaticIndexHelper::exact_ci_helper(const ga::Graph *graph, int n, int &g, ga::Block *T, ga::Blocks *B, int num_vertices)
{
	std::string str_header = form_header(n);

	if (n >= g) return;

	if (T->is_all_set()) {
		B->print(str_header + "ci");
		g = n; return;
	}

	int* map_for_exact_ci = init_map(num_vertices);
	ga::Block* t_complement = compute_t_complement(graph, T, map_for_exact_ci);
	//print_t_comp(map_for_exact_ci, num_vertices, str_header);

	Graph* graph_induced = get_induced_graph(graph, t_complement);

	Blocks* S = compute_S(graph_induced, num_vertices, map_for_exact_ci);
	//S->print(str_header);
	
	int u = map_for_exact_ci[least_in_mis(S, graph->num_vertices)];
	Blocks* S_u = compute_S_u(S, u);

	for(ga::Block* b_n : *S_u) {
		T->union_update(b_n);
		B->push(b_n);
		
		exact_ci_helper(graph, n+1, g, T, B, num_vertices);

		B->pop();
		T->difference_update(b_n);
	}

	delete[](map_for_exact_ci);
	for(ga::Block* block : *S) block->free_element();
	delete(S); delete(S_u);
	t_complement->free_element();
}

ga::Block*
ChromaticIndexHelper::compute_t_complement(const ga::Graph *graph, ga::Block *T, int* map_for_exact_ci)
{
	Block* t_complement = BlockPool::instances[graph->num_vertices]->get();
	T->complement(t_complement);

	int ix_valid=0;
	FORix(graph->num_vertices) {
		// not optimal
		if(t_complement->is_set(ix)) {
			map_for_exact_ci[ix_valid] = ix;
			++ix_valid;
		}
	}
	for(int ix=ix_valid+1; ix<graph->num_vertices; ix++)
		map_for_exact_ci[ix] = -1;
	
	return t_complement;
}


Graph*
ChromaticIndexHelper::get_induced_graph(const Graph* graph, Block* t_complement)
{
	Graph* graph_induced = graph_pool->get(t_complement->size());
	vertices_t_complement->reset();
	FORix(graph->num_vertices) {
		if(t_complement->is_set(ix))
			vertices_t_complement->add(ix);
	}
	graph->copy(graph_induced, vertices_t_complement);
	graph_induced->complement();
	return graph_induced;
}

Blocks*
ChromaticIndexHelper::compute_S(Graph* graph_induced, int num_vertices, int *map_for_exact_ci)
{
	Blocks* S = GraphUtils::max_cliques(graph_induced);
	// NEWLINE;
	// S->print("S_original: ");
	// std::cout << "T': ";
	// FORix(num_vertices) {
	//	if(map_for_exact_ci[ix] == -1) break;
	//	std::cout << map_for_exact_ci[ix] << " ";
	//}
	//NEWLINE; 
	if(ci_debug) {
		std::cout << "S ka block sizes"; NEWLINE;
		for(ga::Block* blk : *S)
			std::cout << blk->size() << "," << blk->capacity() << "  ";
		NEWLINE;
		std::cout << "S::size " << S->size(); NEWLINE;
	}
	
	Blocks* St = new Blocks(S->size(), num_vertices);
	ga::Block *block, *block_t;
	for(int ix_block=0; ix_block<S->size(); ix_block++) {
		block = S->at(ix_block);
		if(false | ci_debug) std::cout << "compute S[" << ix_block << "]: " << block->size() << "," << block->capacity() << " => ";
		
		block_t = St->at(ix_block);
		block_t->reset();

		for(int ix_element=0; ix_element<block->capacity(); ix_element++) {
			// not optimal
			if(block->is_set_(ix_element)) {
				// not optimal
				//std::cout << ix_element+1 << " -> " << map_for_exact_ci[ix_element]+1 << ", ";
				block_t->set_(map_for_exact_ci[ix_element]);
			}
		}
		if(false | ci_debug) {
			block->print();
			std::cout << "  ----> ";
			block_t->print();
			NEWLINE;
		}
		//St->push(block_t);
	}

	for(ga::Block* block : *S) block->free_element();
	delete(S);
	return St;
}

Blocks*
ChromaticIndexHelper::compute_S_u(Blocks* S, int u)
{
	if(ci_debug) {
		std::cout << "compute_S_u for u = " << u << " with ";
		S->print("S: ");
	}
		
	Blocks* S_u = new Blocks();
	for(ga::Block* block: *S) {
		if(block->is_set(u))
			S_u->push(block);
	}
	return S_u;
}

int
ChromaticIndexHelper::least_in_mis(ga::Blocks *S, int n)
{
	FORix(n) hist_S[ix] = 0;
	for(ga::Block* block: *S) {
		for(int element : *block)
			++hist_S[element];
	}
	
	int vertex_min = 0;
	int val_min = hist_S[vertex_min];
	for(int ix=1; ix<n; ix++) {
		if(hist_S[ix] < val_min) {
			val_min = hist_S[ix];
			vertex_min = ix;
		}
	}
	return vertex_min;
}
