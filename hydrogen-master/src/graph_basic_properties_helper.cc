#include "graph_basic_properties_helper.hh"

using namespace ga;

//-- custom headers - start
//-- custom headers - end

GraphBasicPropertiesHelper::GraphBasicPropertiesHelper(GraphBasicProperties* _gbp_mo) {
	gbp_mo = _gbp_mo;
	//- custom -- start
	
	//- custom -- end
}

GraphBasicPropertiesHelper::~GraphBasicPropertiesHelper() {
	//- custom deletions -- start
	free(degree_sequence);
	//- custom deletions -- end
}

void GraphBasicPropertiesHelper::init(int n)
{
	//- custom inits
}


void GraphBasicPropertiesHelper::process_graph(const Graph* graph)
{
	//- the actual logic of how to process this graph
	init_for_graph(graph);
	GraphBasicProperties::Graph_prop_map gbps;

	gbps["order"] = std::to_string(order(graph));
	gbps["num_edges"] = std::to_string(num_edges(graph));

	GraphBasicPropertiesHelper::Degree_properties dp = deg_stuff(graph);
	gbps["deg"] = "[" + 
		std::to_string(std::get<0>(dp)) + "," +
		std::to_string(std::get<1>(dp)) +
		"]";
	gbps["deg_seq"] = std::get<2>(dp);

	gbps["num_comp"] = std::to_string(num_components(graph));
	
	#pragma omp critical(save_parameters)
	{
		gbp_mo->save(graph, gbps);
	}
}

int GraphBasicPropertiesHelper::order(const ga::Graph *graph)
{
	return graph->num_vertices;
}

int GraphBasicPropertiesHelper::num_edges(const ga::Graph *graph)
{
	int _num_edges = 0;
	for(int col=1; col<graph->num_vertices; col++) {
		for(int row=0; row<col; row++) {
			if(graph->is_edge(row,col))
				++_num_edges;
		}
	}
	return _num_edges;
}

GraphBasicPropertiesHelper::Degree_properties
GraphBasicPropertiesHelper::deg_stuff(const ga::Graph *graph)
{
	int min_deg = graph->num_vertices;
	int max_deg = -1;
	FORix(graph->num_vertices) degree_sequence[ix] = 0;
	
	for(int col=1; col<graph->num_vertices; col++) {
		for(int row=0; row<col; row++) {
			if(graph->is_edge(row, col)) {
				degree_sequence[row] += 1;
				degree_sequence[col] += 1;
			}
		}
	}

	ostringstream oss;
	oss << "\"";
	FORix(graph->num_vertices) {
		oss << degree_sequence[ix] << ",";
		if(degree_sequence[ix] < min_deg)
			min_deg = degree_sequence[ix];
		if(degree_sequence[ix] > max_deg)
			max_deg = degree_sequence[ix];
	}
	oss << "\"";

	return std::make_tuple(min_deg, max_deg, oss.str());
}

int GraphBasicPropertiesHelper::num_components(const ga::Graph *graph)
{
	//- ideally
	// FindConnectedComponents::Graphs components =
	// 	FindConnectedComponents::for_graph(graph);
	// return components.size();
	//
	// but we are looking at only connected graphs, so
	return 1;
}

void GraphBasicPropertiesHelper::init_for_graph(const Graph* graph)
{
}

void GraphBasicPropertiesHelper::init_for_new_n(int num_vertices)
{
	if(degree_sequence != nullptr)
		free(degree_sequence);
	degree_sequence = (int*) calloc(num_vertices, sizeof(int));
}

//-- custom methods


