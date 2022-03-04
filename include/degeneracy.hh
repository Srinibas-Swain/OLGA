#ifndef DEGENERACY_HH
#define DEGENERACY_HH

#include "menu_option.hh"

namespace ga {
	class Degeneracy : public MenuOption {
	public:
		Degeneracy();
		~Degeneracy();

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

		void save(const Graph* graph, int degeneracy);
		int get(const Graph* graph);		
		void save_ss_condition(const Graph* graph, bool _ss_condition);
		bool get_ss_condition(const Graph* graph);
		
	protected:
		void init_objects() override;
		
	private:
		int *i_degeneracy_vals;
		bool *b_valid_ss_vals;

		//GraphPool* graph_pool;
		std::string fi_db = "./data/d_db_degeneracy.csv";
		std::string fi_db_ss_condition = "./data/d_db_ss_condition.csv";

		// db queries
		std::unordered_map<int, std::unordered_map<long long, bool> > db_ss_condition;

		std::unordered_map<int, std::unordered_map<long long, int> > db_degen;
		bool absent(int num_vertices, long long certificate);
		bool present(int num_vertices, long long certificate);
		int get(int num_vertices, long long certificate);
		void save(int num_vertices, long long certificate, int degeneracy);
	};
}

#endif /* DEGENERACY_HH */

