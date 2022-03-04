#include <list>
#include "graph_topological_properties_helper.hh"

using namespace ga;

//-- custom headers - start
#include <cstdlib>
#include <limits>

#include "find_connected_components.hh"
//-- custom headers - end

GraphTopologicalPropertiesHelper::GraphTopologicalPropertiesHelper(GraphTopologicalProperties* _gtp_mo) {
	gtp_mo = _gtp_mo;
	//- custom -- start
	
	//- custom -- end
}

GraphTopologicalPropertiesHelper::~GraphTopologicalPropertiesHelper() {
	//- custom deletions -- start
	delete(parents); delete(distances);
	delete(to_visit);
	delete(visited);
	//- custom deletions -- end
}

void GraphTopologicalPropertiesHelper::init(int n)
{
	//- custom inits
}


void GraphTopologicalPropertiesHelper::process_graph(const Graph* graph)
{
	//- the actual logic of how to process this graph
	init_for_graph(graph);
	GraphTopologicalProperties::Graph_prop_map gtps;

	int diam1,rad1;
	int girth = get_girth(graph);
	int circumference;
	if(girth == std::numeric_limits<int>::max())
		circumference = std::numeric_limits<int>::max();
	else
		circumference = get_circumference(graph);
	
	std::tie(diam1,rad1) = get_diameter(graph);
	
	// gtps["girth"]         = (girth == std::numeric_limits<int>::max()) ? "inf" : std::to_string(girth);
	// 
	// int circumference = get_circumference(graph, girth);
	// gtps["circumference"] = (girth == std::numeric_limits<int>::max()) ? "inf" : std::to_string(girth);
	// gtps["diameter"]      = std::to_string(diam1);
	// gtps["radius"]        = std::to_string(rad1);

	
	gtps["girth"]         = girth;
	gtps["circumference"] = circumference;
	gtps["diameter"]      = diam1;
	gtps["radius"]        = rad1;
	

	#pragma omp critical(save_parameters)
	{
		gtp_mo->save(graph, gtps);
	}
}

/// length of a shortest cycle contained in the graph
/// If the graph does not contain any cycles (i.e. it's an acyclic graph),
///    its girth is defined to be infinity
int GraphTopologicalPropertiesHelper::get_girth(const ga::Graph *graph)
{
	int girth = std::numeric_limits<int>::max();
	// std::cout << " has inf: " << std::numeric_limits<int>::has_infinity << std::endl;
	// std::cout << " inf: " << std::numeric_limits<int>::infinity() << std::endl;
	// std::cout << " max: " << std::numeric_limits<int>::max() << std::endl;
	// std::cout << " max < inf ? " << (std::numeric_limits<double>::max() < std::numeric_limits<int>::infinity()) << std::endl;
	int current;
	
	//std::cout << "#" << graph->graph_id; NEWLINE;
	for(int vertex=0; vertex<graph->num_vertices; vertex++) {
		init_girth_for_vertex(graph->num_vertices);

		to_visit->set_(vertex);
		distances[vertex] = 0;

		while(to_visit->is_not_empty_()) {
			//to_visit->print(); NEWLINE;
			current = to_visit->pop_head_();
			// std::cout << " size: " << to_visit->size() << " size_: " << to_visit->size_();
			// std::cout << " current: " << current << std::endl;
			visited->append_(current, "visited:");

			for(int nbr=0; nbr<graph->num_vertices; nbr++) {
				if(!graph->is_edge(current, nbr)) continue;
				if(nbr == parents[current])       continue;
				if(visited->contains_(nbr)) {
					girth = min(girth, distances[nbr]+distances[current]+1);
				} else {
					distances[nbr] = distances[current]+1;
					parents[nbr]   = current;
					to_visit->append_(nbr, "to_visit:");
				}
			}
		}
	}
	return girth;
}

void GraphTopologicalPropertiesHelper::init_girth_for_vertex(int num_vertices)
{
	visited->reset();
	to_visit->reset();
	FORix(num_vertices) {
		parents[ix]   = -1;
		distances[ix] =  0;
	}
	
}

int GraphTopologicalPropertiesHelper::get_circumference(const ga::Graph *graph)
{
	if(is_two_degree_graph(graph)) {
		// std::cout << graph->graph_id << ": 2 degree graph" << std::endl;
		return graph->num_vertices;
	}

	std::vector<int> cut_vertices = graph->get_cut_vertices();
	if(cut_vertices.size() > 0) {
		int circum_cut = get_circumference_from_cut_vertices(graph, cut_vertices);
		std::cout << graph->graph_id << ": from cut-vertices : "
				  << circum_cut << std::endl;
		return circum_cut;
	}

	Graph* graph_child = graph->clone();
	int circumference_max = -2, circumference_child;
	
	for(int vertex1=0; vertex1 < graph->num_vertices; vertex1++) {
		for(int vertex2=1; vertex2 < graph->num_vertices; vertex2++) {
			if(! graph->is_edge(vertex1, vertex2))
				continue;

			graph_child->remove_edge(vertex1, vertex2);
			// is this needed?
			graph_child->construct_graph_id();
			circumference_child = gtp_mo->get(graph_child)["circumference"];

			if((circumference_child != std::numeric_limits<int>::max()) &&
			   (circumference_child > circumference_max))
				circumference_max = circumference_child;
			
			graph_child->add_edge(vertex1, vertex2);
		}
	}
	delete(graph_child);

	return circumference_max;
}

// split the graph at a random cut vertex
// find the circumferences of the resulting subgraphs
// find the max of the finite circumferences 
int GraphTopologicalPropertiesHelper::get_circumference_from_cut_vertices(const Graph* graph, std::vector<int> cut_vertices)
{
	Graph *graph_clone = graph->clone();

	// select a random cut vertex
	int cut_vertex = cut_vertices[rand() % cut_vertices.size()];
	std::cout << "gtph: cv: " << cut_vertex << std::endl;

	graph_clone->reset_vertex(cut_vertex);
	int circumference_max = -1, circumference_subgraph;
	std::vector<Graph*> sub_graphs = FindConnectedComponents::for_graph(graph_clone);
	std::cout << "gtph: subgraphs: " << sub_graphs.size() << std::endl;
	
	for(Graph* sub_graph : sub_graphs) {
		sub_graph->construct_graph_id();
		circumference_subgraph = gtp_mo->get(sub_graph)["circumference"];

		if(circumference_subgraph == std::numeric_limits<int>::max())
			continue;
		
		if(circumference_subgraph > circumference_max)
			circumference_max = circumference_subgraph;
	}
	
	for(Graph* sub_graph : sub_graphs) delete(sub_graph);
	delete(graph_clone);

	return circumference_max;
}

// check if min and max degrees are equal to 2
bool GraphTopologicalPropertiesHelper::is_two_degree_graph(const Graph* graph)
{
	for(auto degree : graph->get_degrees()) {
		if(degree != 2)
			return false;
	}
	return true;
}




std::tuple<int, int> GraphTopologicalPropertiesHelper::get_diameter(const ga::Graph *graph)
{ 
	int distance[graph->num_vertices][graph->num_vertices]; // Distances between nodes
	int eccentricity[graph->num_vertices];
	int diam=std::numeric_limits<int>::min();; // Diamater of graph
	int rad=std::numeric_limits<int>::max();//Radius of a graph
	
	FORix(graph->num_vertices) {
		eccentricity[ix] =  0;
	}

	for(int vertex=0; vertex<graph->num_vertices; vertex++) {
		for(int adj_vertex=0; adj_vertex<graph->num_vertices; adj_vertex++) {
			if(graph->is_edge(vertex, adj_vertex))
				distance[vertex][adj_vertex]=1;
			else
				distance[vertex][adj_vertex]=std::numeric_limits<int>::max();
			distance[vertex][vertex]=0;
		}
	}

	for(int vertex=0; vertex<graph->num_vertices; vertex++) {
		for(int adj_vertex=0; adj_vertex<graph->num_vertices; adj_vertex++) {
			for(int next_vertex=0; next_vertex<graph->num_vertices; next_vertex++) {
				if(next_vertex != adj_vertex)
					if(distance[next_vertex][vertex]!=std::numeric_limits<int>::max() && distance[vertex][adj_vertex]!=numeric_limits<int>::max())
						distance[next_vertex][adj_vertex] = min(distance[next_vertex][adj_vertex],distance[next_vertex][vertex] + distance[vertex][adj_vertex]);
			}
		}
	}

	//        for(int vertex=0; vertex<graph->num_vertices; vertex++) {
	//                init_eccentricity_for_vertex(graph->num_vertices);
	//        }

	for(int vertex=0; vertex<graph->num_vertices; vertex++) {
		for(int adj_vertex=0; adj_vertex<graph->num_vertices; adj_vertex++) {
			eccentricity[vertex]=max(static_cast<int>(eccentricity[vertex]),distance[vertex][adj_vertex]);
		}
	}
	for(int vertex=0; vertex<graph->num_vertices; vertex++) {
		diam = max(diam, eccentricity[vertex]);
		rad = min(rad, eccentricity[vertex]);
	}
	return std::make_tuple(diam,rad);
	
}


//int GraphTopologicalPropertiesHelper::get_radius(const ga::Graph *graph)
//{
//	return -1;	
//}

void GraphTopologicalPropertiesHelper::init_for_graph(const Graph* graph)
{
}

void GraphTopologicalPropertiesHelper::init_for_new_n(int num_vertices)
{
	if(visited != nullptr)   delete(visited);
	if(to_visit != nullptr) delete(to_visit);
	visited  = new VertexSet(num_vertices);
	to_visit = new VertexSet(num_vertices);

	if(parents != nullptr) free(parents);
	if(distances != nullptr) free(distances);
	parents   = (int*) calloc(num_vertices, sizeof(int));
	distances = (int*) calloc(num_vertices, sizeof(int));
}

//-- custom methods


