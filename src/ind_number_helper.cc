#include "ind_number_helper.hh"

using namespace ga;

//-- custom headers - start
//-- custom headers - end

IndNumberHelper::IndNumberHelper(IndNumber* _ind_num_mo) {
	ind_num_mo = _ind_num_mo;
	//- custom -- start
	graph_pool = new GraphPool();
	//- custom -- end
}

IndNumberHelper::~IndNumberHelper() {
	//- custom deletions -- start
	delete(graph_pool);
	//- custom deletions -- end
}

void IndNumberHelper::init(int n)
{
	//- custom inits
	graph_pool->init(n+1);
}


void IndNumberHelper::process_graph(const Graph* graph)
{
	//- the actual logic of how to process this graph

	init_for_graph(graph);

	int ind_num = find_ind_number(graph);
	int clique_number = -1;
	int vertex_cover = graph->num_vertices - ind_num;

	// save the fuckers
	Triplet ind_cli_vc = std::make_tuple(ind_num, clique_number, vertex_cover);
	#pragma omp critical(save_parameters)
	{
		ind_num_mo->save(graph, ind_cli_vc);
	}
}

void IndNumberHelper::init_for_graph(const Graph* graph)
{
}

//-- custom methods
int IndNumberHelper::find_ind_number(const ga::Graph *graph)
{
	int ind_num_max = -1, ind_num_vertex;
	int ind_num_sum=0;
	for(int vertex=0; vertex<graph->num_vertices; vertex++) {
		ind_num_vertex = max(find_ind_number_wo_vertex(graph, vertex),
							 find_ind_number_wo_blanket(graph, vertex)+1);
		ind_num_sum += ind_num_vertex;
		ind_num_max = max(ind_num_max, ind_num_vertex);
	}
	return ind_num_max;
	//return ind_num_sum;
}

int IndNumberHelper::find_clique_number(int num_vertices, long long graph_id)
{
	Graph* graph_c = graph_pool->get(num_vertices);
	graph_c->imbibe_graph_id(graph_id);
	graph_c->complement();
	int ind_num_max = -1, ind_num_curr, ind_num_sum=0;
	for(auto component : FindConnectedComponents::for_graph(graph_c)) {
		ind_num_curr = (component->num_vertices == 1) ? 1 : std::get<0>(ind_num_mo->get(component));
		ind_num_max = max(ind_num_max, ind_num_curr);
		ind_num_sum += ind_num_curr;
	}
	//return ind_num_max;
	return ind_num_sum;
}

// find the ind-number of the graph without the vertex
// remove the vertex and
// find the max ind number among the connected components
int IndNumberHelper::find_ind_number_wo_vertex(const ga::Graph *graph, int vertex)
{
	Graph* graph_child = graph_pool->get(graph->num_vertices - 1);
	graph->copy_sans_vertex_into(graph_child, vertex);
	return find_max_ind_number_components(FindConnectedComponents::for_graph(graph_child));
}

// find the ind-number of the graph without the vertex blanket
// remove the vertex blanket and
// find the max ind number among the connected components
int IndNumberHelper::find_ind_number_wo_blanket(const ga::Graph *graph, int vertex)
{
	Graph* graph_child = graph_pool->get(graph->num_vertices-1-graph->number_of_nbrs(vertex));
	graph->copy_sans_blanket_into(graph_child, vertex);
	return find_max_ind_number_components(FindConnectedComponents::for_graph(graph_child));
}


int IndNumberHelper::find_max_ind_number_components(std::vector<Graph*> components)
{
	int ind_num_max = -1, ind_num_comp;
	int ind_num_sum = 0;
	for(auto component : components) {
		if(component->num_vertices == 1)
			ind_num_comp = 1;
		else
			ind_num_comp = std::get<0>(ind_num_mo->get(component));
		
		ind_num_max = max(ind_num_max, ind_num_comp);
		ind_num_sum += ind_num_comp;
		delete(component);
	}
	//return ind_num_max;
	return ind_num_sum;
}



