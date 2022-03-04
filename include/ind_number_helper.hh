#ifndef IND_NUMBER_HELPER_HH
#define IND_NUMBER_HELPER_HH

#include "menu_option.hh"
#include "menu_option_helper.hh"

#include "ind_number.hh"
#include "utils.hh"

//- other headers -- start
#include "graph_pool.hh"
#include "find_connected_components.hh"
//- other headers -- end

namespace ga {
	class IndNumberHelper : public MenuOptionHelper {
	public:
		IndNumberHelper(IndNumber* ind_num_mo);
		~IndNumberHelper();

		void init(int n) override;
		void process_graph(const Graph*);

		//- public functions -- start
		int find_clique_number(int num_vetices, long long cert);
		//- public functions -- end
	  
	private:
		IndNumber* ind_num_mo;

		void init_for_graph(const Graph* graph);
		
		//- custom private -- start
		GraphPool* graph_pool;

		int find_ind_number(const Graph* graph);
		int find_ind_number_wo_vertex(const Graph* graph, int vertex);
		int find_ind_number_wo_blanket(const Graph* graph, int vertex);
		int find_max_ind_number_components(std::vector<Graph*> components);
		//- custom private -- end
	};
}

#endif /* IND_NUMBER_HELPER_HH */

