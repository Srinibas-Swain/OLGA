#ifndef GENUS_HELPER_HH
#define GENUS_HELPER_HH

#include "menu_option.hh"
#include "menu_option_helper.hh"

#include "genus.hh"
#include "utils.hh"

//- other headers -- start
#include "find_connected_components.hh"
#include "graph_pool.hh"
//- other headers -- end

namespace ga {
	class GenusHelper : public MenuOptionHelper {
	public:
		GenusHelper(Genus* genus_mo);
		~GenusHelper();

		void init(int n) override;
		void process_graph(const Graph*);

		//- public functions -- start
		//- public functions -- end
	  
	private:
		Genus* genus_mo;

		void init_for_graph(const Graph* graph);
		
		//- custom private -- start
		GraphPool* graph_pool;
		std::pair<int,int> genus_range_of_components(const Graph* graph);
		int process_graph_child(const Graph* graph_child);
		//- custom private -- end
	};
}

#endif /* GENUS_HELPER_HH */

