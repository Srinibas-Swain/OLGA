#ifndef ALL_MAX_INDUCED_SUBGRAPHS_HELPER_HH
#define ALL_MAX_INDUCED_SUBGRAPHS_HELPER_HH

#include <string>
#include <vector>

//#include "all_max_induced_subgraphs.hh"
#include "block.hh"
#include "certificate_cache2.hh"
#include "frequency_counter.hh"
#include "graph.hh"
#include "graph_pool.hh"
#include "menu_option.hh"
#include "menu_option_helper.hh"

namespace ga {
	class AllMaxInducedSubgraphsHelper : public MenuOptionHelper {
	public:
		AllMaxInducedSubgraphsHelper();
		~AllMaxInducedSubgraphsHelper();

		//void process_graph(const Graph*, std::map<long long, bool>) override;
		//vector<long long> process_graph(const Graph *, const AllMaxInducedSubgraphs* amis);
		//std::pair<vector<std::pair<int, long long> >, long long> process_graph(const Graph *, const MenuOption* menu_option);
		mfcis::V_Fisgs_Count process_graph(const Graph *, const MenuOption* menu_option);
		void print_cache();
	private:
		Graph* graph_tmp;
		GraphPool* graph_pool;
		//todo
		void copy_graph(Graph* src, Graph* dst, vector<int> vertices);
	};
}

#endif /* ALL_MAX_INDUCED_SUBGRAPHS_HELPER_HH */

