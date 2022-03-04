#ifndef TREE_WIDTH_HELPER_HH
#define TREE_WIDTH_HELPER_HH

#include "menu_option.hh"
#include "menu_option_helper.hh"

#include "tree_width.hh"
#include "utils.hh"

//- other headers -- start
#include "find_connected_components.hh"
#include "graph_pool.hh"
//- other headers -- end

namespace ga {
	class TreeWidthHelper : public MenuOptionHelper {
	public:
		TreeWidthHelper(TreeWidth* tw_mo);
		~TreeWidthHelper();

		void init(int n) override;
		void process_graph(const Graph*);

		//- public functions -- start
		//- public functions -- end
	  
	private:
		TreeWidth* tw_mo;

		void init_for_graph(const Graph* graph);
		
		//- custom private -- start
		GraphPool* graph_pool;
		int tw_lb_max, tw_ub_min;

		int process_removed_node(const Graph* graph_parent, Graph* graph_child);
		//- custom private -- end
	};
}

#endif /* TREE_WIDTH_HELPER_HH */

