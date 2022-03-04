#ifndef GRAPH_TOPOLOGICAL_PROPERTIES_HELPER_HH
#define GRAPH_TOPOLOGICAL_PROPERTIES_HELPER_HH

#include "menu_option.hh"
#include "menu_option_helper.hh"

#include "graph_topological_properties.hh"
#include "utils.hh"

//- other headers -- start
#include "vertex_set.hh"
//- other headers -- end

namespace ga {
	class GraphTopologicalPropertiesHelper : public MenuOptionHelper {
	public:
		GraphTopologicalPropertiesHelper(GraphTopologicalProperties* _gtp_mo);
		~GraphTopologicalPropertiesHelper();

		void init(int n) override;
		void process_graph(const Graph*);
		void init_for_new_n(int num_vertices) override;

		//- public functions -- start
		//- public functions -- end
	  
	private:
		GraphTopologicalProperties* gtp_mo;

		void init_for_graph(const Graph* graph);
		
		//- custom private -- start
		int get_girth(const Graph* graph);
		void init_girth_for_vertex(int num_vertices);

		int get_circumference(const Graph* graph);
		int get_circumference_from_cut_vertices(const Graph*, std::vector<int>);
		bool is_two_degree_graph(const Graph* graph);

		std::tuple<int, int> get_diameter(const Graph* graph);
		//int get_radius(const Graph* graph);

		VertexSet *visited, *to_visit;
		int *parents, *distances;
		//- custom private -- end
	};
}

#endif /* GRAPH_TOPOLOGICAL_PROPERTIES_HELPER_HH */

