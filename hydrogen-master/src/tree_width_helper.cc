#include "tree_width_helper.hh"

using namespace ga;

//-- custom headers - start
//-- custom headers - end

TreeWidthHelper::TreeWidthHelper(TreeWidth* _tw_mo) {
	tw_mo = _tw_mo;
	//- custom -- start
	graph_pool = new GraphPool();
	//- custom -- end
}

TreeWidthHelper::~TreeWidthHelper() {
	//- custom deletions -- start
	delete(graph_pool);
	//- custom deletions -- end
}

//- init once before anything starts
void TreeWidthHelper::init(int n)
{
	//- custom inits
	graph_pool->init(n+1);
}



// tree_Widths = []
// for v in vertices:
//   graph_new = graph \ vertex
//   tw_components = [comp.tree_width for comp in graph_new.connected_components]
//   tw_current = tw_components.max
// 	 tree_widths << tw_current
// lb = tree_widths.max
// ub = tree_widths.min + 1
// if lb == ub:
//   print("bounds match", lb)
// else:
//	 print("bounds difft", lb, ub)
// graph.tree_width = ub
void TreeWidthHelper::process_graph(const Graph* graph)
{
	//- the actual logic of how to process this graph

	//init_for_graph(graph);
	int tw_child;
	tw_lb_max = -1;
	tw_ub_min = graph->num_vertices+2;

	// todo: init graph pool
	Graph* graph_child = graph_pool->get(graph->num_vertices);
	graph->copy(graph_child);
	
	FORix(graph->num_vertices) {
		graph_child->reset_vertex(ix);

		tw_child =  process_removed_node(graph, graph_child);
		if(tw_child > tw_lb_max)	tw_lb_max = tw_child;
		if(tw_child < tw_ub_min)	tw_ub_min = tw_child;

		graph_child->copy_edges_from(graph, ix);
	}

	tw_ub_min += 1;
	Bounds bounds_curr = std::make_pair(tw_lb_max, tw_ub_min);
	#pragma omp critical(tw_save_parameters)
	{
		tw_mo->save(graph, bounds_curr);
	}
}

void TreeWidthHelper::init_for_graph(const Graph* graph)
{
}

//-- custom methods
int TreeWidthHelper::process_removed_node(const Graph* graph_parent,
										  Graph* graph_child)
{
	int tw_max = -1;
	long long cert_comp;
	Bounds tw_curr;
	
	for(auto component : FindConnectedComponents::for_graph(graph_child)) {
		cert_comp = tw_mo->get_certificate(component);
		tw_curr = tw_mo->get(component);
		if(tw_curr.second > tw_max) {
			tw_max = tw_curr.second;
		}
	}
	return tw_max;
}

