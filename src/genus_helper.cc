#include "genus_helper.hh"

using namespace ga;

//-- custom headers - start
//-- custom headers - end

GenusHelper::GenusHelper(Genus* _genus_mo) {
	genus_mo = _genus_mo;
	//- custom -- start
	graph_pool = new GraphPool();
	//- custom -- end
}

GenusHelper::~GenusHelper() {
	//- custom deletions -- start
	delete(graph_pool);
	//- custom deletions -- end
}

void GenusHelper::init(int n)
{
	//- custom inits
	graph_pool->init(n+1);
}


// the graph is always connected:: see MenuOption::process_graphs()
void GenusHelper::process_graph(const Graph* graph)
{
	//- the actual logic of how to process this graph

	init_for_graph(graph);
	bool is_connected = true;

	int gamma, gamma_wo_edge;
	int gamma_min = -1, gamma_max = -1;
	if(graph->is_tree(is_connected)) {
		gamma_min = gamma_max = gamma = 0;
	} else {
		std::pair<int,int> gamma_min_max = genus_range_of_components(graph);
		gamma_min = std::get<0>(gamma_min_max);
		gamma_max = std::get<1>(gamma_min_max);
		if(graph->num_vertices <= 5) {
			if(graph->num_vertices==5 && graph->is_k_n(genus_mo->cert_cache->get(graph)))
				gamma = 1;
			else
				gamma = 0;
		} else {
			gamma = gamma_min+1;
		}
	}
	Triplet min_max_gamma = std::make_tuple(gamma_min, gamma_max, gamma);
	#pragma omp critical(save_parameters)
	{
		genus_mo->save(graph, min_max_gamma);
	}
}

void GenusHelper::init_for_graph(const Graph* graph)
{
}

//-- custom methods
std::pair<int,int> GenusHelper::genus_range_of_components(const ga::Graph *graph)
{
	long long gamma_min = genus_mo->ncr_get(graph->num_vertices, 2)+1;
	int gamma_max = -1;
	int gamma_wo_edge;
	Graph* graph_child = graph_pool->get(graph->num_vertices);
	graph->copy(graph_child);
	
	for(int row=0; row<graph->num_vertices; row++) {
		for(int col=row+1; col<graph->num_vertices; col++) {
			if(!graph->is_edge(row, col)) {
				continue;
			}

			graph_child->remove_edge(row, col);
			graph_child->construct_graph_id();

			gamma_wo_edge = process_graph_child(graph_child);
			if(gamma_wo_edge < gamma_min) gamma_min = gamma_wo_edge;
			if(gamma_wo_edge > gamma_max) gamma_max = gamma_wo_edge;

			graph_child->set_edge(row, col);
		}
	}
	return std::make_pair(int(gamma_min), gamma_max);
}


int GenusHelper::process_graph_child(const ga::Graph *graph_child)
{
	int gamma_max = -1, gamma_curr;
	Triplet min_max_gamma;
	for(auto component : FindConnectedComponents::for_graph(graph_child) ) {
		if(component->num_vertices == 1) {
			gamma_curr = 0;
		} else {
			min_max_gamma = genus_mo->get(component);
			gamma_curr = std::get<2>(min_max_gamma);
		}

		if(gamma_curr>gamma_max) gamma_max = gamma_curr;

		delete(component);
	}
	return gamma_max;
}


