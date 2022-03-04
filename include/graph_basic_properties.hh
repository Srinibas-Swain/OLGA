#ifndef GRAPH_BASIC_PROPERTIES_HH
#define GRAPH_BASIC_PROPERTIES_HH

#include "menu_option.hh"
#include <unordered_map>

namespace ga {
	class GraphBasicProperties : public MenuOption {
	public:
		typedef typename std::unordered_map<std::string, std::string> Graph_prop_map;
		GraphBasicProperties();
		~GraphBasicProperties();

		void print_args_input_info() override;
		void get_args_input(char split_char) override;
		void init() override;
		void init_for_new_n(int n) override;
		void process() override;
		void process_graph(const Graph* graph) override;
		void post_processing() override;
		void print_args() override;

		void init_base_cases();

		void read_db_from_file();
		bool is_db_present();
		void write_db_to_file();

		void save(const Graph* graph, Graph_prop_map);
		Graph_prop_map get(const Graph* graph);
		//- custom stuff

	protected:
		void init_objects() override;
		
	private:
		std::string fi_db = "./data/d_db_graph_basic_prop_map.csv";
		std::unordered_map<int, std::unordered_map<long long, Graph_prop_map > > db_gbp;

		Graph_prop_map get(int num_vertices, long long certificate);
		void save(int num_vertices, long long certificate, Graph_prop_map);
	};
}

#endif /* GRAPH_BASIC_PROPERTIES_HH */

