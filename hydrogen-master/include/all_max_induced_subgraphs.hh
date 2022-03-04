#ifndef ALL_MAX_INDUCED_SUBGRAPHS_HH
#define ALL_MAX_INDUCED_SUBGRAPHS_HH

#include <unordered_map>
#include <vector>

#include "all_max_induced_subgraphs_helper.hh"
#include "certificate.hh"
#include "certificate_cache2.hh"
#include "frequent_induced_subgraph.hh"
#include "menu_option.hh"


namespace ga {
	class AllMaxInducedSubgraphs : public MenuOption {
	public:
		AllMaxInducedSubgraphs();
		~AllMaxInducedSubgraphs();

		void print_args_input_info() override;
		void get_args_input(char split_char) override;
		void init() override;
		void init_for_new_n(int n) override;
		void process() override;
		void process_graph(const Graph* graph) override;
		void post_processing() override;
		void print_args() override;
		void print_fisgs();

	protected:
		void init_objects() override;
		void init_base_cases();
		
	private:
		void clean_objects();
		bool is_fisg_present(int num_vertices, long long cert);

		std::unordered_map<int,
						   std::unordered_map<long long,
											  mfcis::V_Fisgs_Count> > fisgs_by_n__cert;
	};
}

#endif /* ALL_MAX_INDUCED_SUBGRAPHS_HH */


