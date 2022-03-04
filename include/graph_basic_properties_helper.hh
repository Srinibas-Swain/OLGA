#ifndef GRAPH_BASIC_PROPERTIES_HELPER_HH
#define GRAPH_BASIC_PROPERTIES_HELPER_HH

#include "menu_option.hh"
#include "menu_option_helper.hh"

#include "find_connected_components.hh"
#include "graph_basic_properties.hh"
#include "utils.hh"

//- other headers -- start
//- other headers -- end

namespace ga {
	class GraphBasicPropertiesHelper : public MenuOptionHelper {
	public:
		typedef typename std::tuple<int, int, std::string> Degree_properties;
		GraphBasicPropertiesHelper(GraphBasicProperties* _gbp_mo);
		~GraphBasicPropertiesHelper();

		void init(int n) override;
		void process_graph(const Graph*);
		void init_for_new_n(int num_vertices) override;

		//- public functions -- start
		//- public functions -- end
	  
	private:
		GraphBasicProperties* gbp_mo;

		void init_for_graph(const Graph* graph);
		
		//- custom private -- start
		///-- Order(# of vertices)
		int order(const Graph* graph);
		///-- # of Edges
		int num_edges(const Graph* graph);
		///-- Min Degree
		///-- Max Degree
		///-- Degree Sequence
		Degree_properties deg_stuff(const Graph* graph);
		///-- Number Of Components
		int num_components(const Graph* graph);


		int* degree_sequence;
		//- custom private -- end
	};
}

#endif /* GRAPH_BASIC_PROPERTIES_HELPER_HH */

