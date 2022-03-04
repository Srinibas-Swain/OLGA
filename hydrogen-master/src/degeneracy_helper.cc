#include "degeneracy_helper.hh"

using namespace ga;
#include <iostream>
#include <string>

#include "find_connected_components.hh"

DegeneracyHelper::DegeneracyHelper(Degeneracy* _degen_mo) {
	degen_mo = _degen_mo;
	//-- custom
	num_vertices = 0;
	L = new NodeList(-1);
	graph_child = new Graph(1);
}

DegeneracyHelper::~DegeneracyHelper() {
	free_data(); // ??
	free(graph_child);
	delete(L);
}

void DegeneracyHelper::init(int n)
{
}


/*
 * 1. Initialize an output list L.
 *
 * 2. Compute a number d_v for each vertex v in G, the number of neighbors of v that are not already in L
 *      - Initially, these numbers are just the degrees of the vertices.
 *
 * 3. Initialize an array D such that D[i] contains a list of the vertices v that are not already in L for which
 * d v  = i.
 *
 * 4. Initialize k to 0.
 *
 * 5. Repeat n times:
 *     (a) Scan the array cells D[0], D[1], ... until finding an i for which D[i] is nonempty.
 *
 *     (b) Set k to max(k,i)
 *
 *     (c) Select a vertex v from D[i].
 *
 *     (d) Add v to the beginning of L and remove it from D[i].
 *
 *     (e) For each neighbor w of v not already in L
 *           (I) subtract one from d w and move w to the cell of D corresponding to the new value of d_w
 *
 * At the end of the algorithm
 *   - k contains the degeneracy of G
 *   - L contains a list of vertices in a optimal ordering for the coloring number
 *   - The i-cores of G are the prefixes of L consisting of the vertices added to L after k first takes a value greater than or equal to i.
*/
void DegeneracyHelper::process_graph(const Graph* graph)
{
	bool _b_debug = false;
	//-- custom
	init_for_graph(graph);
	//_b_debug = (graph->graph_id == 3887);
	if(_b_debug) graph->print("");
	int k=0;
	int ix_ne=0;
	Node *v, *w;

	FORix(num_vertices) {
		if(_b_debug) std::cout << " ------- " << std::endl;
		if(_b_debug) print_D();
		if(_b_debug) L->print("L");
		
		ix_ne = index_D_non_empty(ix_ne);
		if(_b_debug) std::cout << "ix_ne: " << ix_ne << std::endl;

		k = max(k, ix_ne);

		v = pop_from_D(ix_ne);
		if(_b_debug) std::cout << "popped " << v->val << std::endl;
		if(_b_debug) print_D();

		L->prepend(v);

		// process nbrs
		for(int ix_w=0; ix_w<num_vertices; ix_w++) {
			w = nodes[ix_w];

			if(valid_w(graph, v, w)) {
				if(_b_debug)
					std::cout << "update D with w:" << w->val << std::endl;
				update_D(degrees[ix_w], degrees[ix_w]-1, w);
				if(_b_debug)print_D();

				--degrees[ix_w];
				ix_ne = min(degrees[ix_w],ix_ne);
			}
		}
	}

	#pragma omp critical(save_parameters)
	{
		degen_mo->save(graph, k);
	}
	bool b_valid_ss_condition = validate_ss_condition(graph, k);
	#pragma omp critical(degen_validate_ss_condition)
	{
		degen_mo->save_ss_condition(graph, b_valid_ss_condition);
	}
}

//-- custom methods

bool DegeneracyHelper::validate_ss_condition(const ga::Graph *graph, int k,
											 bool _b_debug, bool _b_debug2)
{
	init_gchild_from(graph);
	std::ostringstream oss_out;
	bool b_valid = true;

	if(_b_debug2) graph->print2("g");

	int k_max_components;
	FORix(graph->num_vertices) {
		graph_child->reset_vertex(ix);
		if(_b_debug2) oss_out << std::endl << graph_child->get_string("c");

		if(_b_debug) oss_out << "{";
		k_max_components = max_degeneracy_components(graph_child, oss_out, _b_debug2);
		if(_b_debug) oss_out << k_max_components << "}";
		
		// k(G \ v) <= k(G) <= k(G \ v) + 1
		b_valid = b_valid && ((k == k_max_components) || (k == (k_max_components+1)));
		
		graph_child->copy_edges_from(graph, ix);
	}

	if(_b_debug) {
		#pragma omp critical(degen_validate_ss_condition_out)
		{
			std::cout << oss_out.str();	
		}
	}
	return b_valid;
}

int DegeneracyHelper::max_degeneracy_components(const ga::Graph *graph_child,
												std::ostringstream& oss_out, bool _b_debug)
{
	std::vector<Graph*> conn_components = FindConnectedComponents::for_graph(graph_child);
	long long certificate;
	int k_max = 0, k_current;
	if(_b_debug) {
		std::cout << ".";
	}
	for(auto component : conn_components) {
		component->construct_graph_id();
		if(component->num_vertices == 1) {
			k_current = 0;
		} else {
			// if(component->graph_id == 42)
			// 	component->print();
			certificate = degen_mo->generate_certificate(component);
			k_current = degen_mo->get(component);
		}
		
		if(_b_debug) {
			if(component->graph_id==42) {
				graph_child->print("42 ka baap");
				component->print("dh:mdc==42");
				std::cout << component->num_vertices << "#" << component->graph_id << "-" << component->connected() << "-" <<
					certificate << "-" << k_current << " " << std::endl;
			}
			else {
				component->print("dh:mdc");
			}
		}
		
		if(_b_debug) oss_out << component->num_vertices << "#" << component->graph_id << "-" << component->connected() << "-" <<
						 certificate << "-" << k_current << " ";
		if(_b_debug) std::cout << certificate << "-" << k_current << ", ";
		k_max = max(k_max, k_current);
	}
	if(_b_debug) oss_out << "::";
	if(_b_debug) std::cout << std::endl;
	return k_max;
}

void DegeneracyHelper::init_gchild_from(const ga::Graph *graph)
{
	if(graph_child->num_vertices != graph->num_vertices) {
		free(graph_child);
		graph_child = new Graph(graph->num_vertices);
	}
	graph_child->reset_all_edges();
	graph->copy(graph_child);
}

int DegeneracyHelper::index_D_non_empty(int ix_ne)
{
	for(int ix=ix_ne; ix<num_vertices; ix++) {
		if(D[ix] != nullptr) {
			return ix;
		}
	}
	throw std::runtime_error("no empty block in D");
}

Node* DegeneracyHelper::pop_from_D(int ix_ne)
{
	validate_non_empty_D(ix_ne);
	Node *v = D[ix_ne];
	D[ix_ne] = v->next;
	if(D[ix_ne] != nullptr)
		D[ix_ne]->prev = nullptr;
	return v;
}

void DegeneracyHelper::update_D(int deg_old, int deg_new, Node *w)
{
	// rm w
	// a -> w -> b
	//
	// a -> b
	if(w->prev != nullptr) {
		w->prev->next = w->next;
	} else { // w is at the head
		if(D[deg_old] != w) {
			std::ostringstream oss_err;
			print_D();
			oss_err << "|g|=" << num_vertices << " gid:" << graph_id;
			oss_err << "  D[deg_old] != w ";
			throw std::runtime_error(oss_err.str());
		}
		D[deg_old] = w->next;
	}
	// a <- b
	if(w->next != nullptr)
		w->next->prev = w->prev;

	// add w
	w->next = D[deg_new];
	if(w->next != nullptr)
		w->next->prev = w;
	w->prev = nullptr;
	D[deg_new] = w;
}


void DegeneracyHelper::validate_non_empty_D(int ix)
{
	validate_bounds(ix);
	if(D[ix] != nullptr) {
		return;
	}
	std::ostringstream oss_err;
	oss_err << "D[" << ix << "]" << "is null";
	throw std::runtime_error(oss_err.str());
}

// skip if w is not a nbr or if L has w already
// perf: which check shud be first?
bool DegeneracyHelper::valid_w(const Graph *graph, Node *v, Node *w)
{
	return graph->is_edge(v->val, w->val) &&
		!L->is_present(w);
}

void DegeneracyHelper::validate_bounds(int ix)
{
	if(ix >= num_vertices) {
		std::ostringstream oss_err;
		oss_err << "DegeneracyHelper bounds error: " <<
			ix << " >= " << num_vertices;
		throw std::runtime_error(oss_err.str());
	}
}

void DegeneracyHelper::init_for_graph(const Graph* graph)
{
	int _num_vertices = graph->num_vertices;
	if(num_vertices != _num_vertices) {
		reallocate(_num_vertices);
		num_vertices = _num_vertices;
	}

	L->init(num_vertices);
	FORix(num_vertices) {
		nodes[ix]->reset();
		degrees[ix] = -1;
		D[ix] = NULL;
	}
	std::vector<int> degrees_g = graph->get_degrees();
	int degree;

	FORix(num_vertices) {
		degree = degrees_g[ix];
		if(D[degree] == nullptr) {
			D[degree] = nodes[ix];
		} else {
			D[degree]->prev = nodes[ix];
			nodes[ix]->next = D[degree];
			D[degree] = nodes[ix];
		}
		degrees[ix] = degree;
	}
	graph_id = graph->graph_id;
}

void DegeneracyHelper::reallocate(int _num_vertices)
{
	free_data();

	nodes = (Node**) calloc(_num_vertices, sizeof(Node*));
	degrees = (int*) calloc(_num_vertices, sizeof(int));
	D = (Node**) calloc(_num_vertices, sizeof(Node*));

	FORix(_num_vertices) {
		nodes[ix] = new Node(ix);
	}
}

void DegeneracyHelper::free_data()
{
	if(num_vertices == 0)
		return;

	FORix(num_vertices)
		delete(nodes[ix]);

	free(D);
	free(degrees);
	free(nodes);
}

void DegeneracyHelper::print_D() const
{
	Node *node;
	std::cout << "D: ";
	FORix(num_vertices) {
		std::cout << "{" << ix << ": ";
		node = D[ix];
		while(node != nullptr) {
			std::cout << node->val << ",";
			node = node->next;
		}
		std::cout << " } ";
	}
	std::cout << std::endl;
}
