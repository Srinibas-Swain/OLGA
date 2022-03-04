#ifndef TREE_WIDTH_HH
#define TREE_WIDTH_HH

#include "menu_option.hh"

namespace ga {
	typedef typename std::pair<int,int> Bounds;

	class TreeWidth : public MenuOption {
	public:
		TreeWidth();
		~TreeWidth();

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

		void save(const Graph* graph, Bounds);
		Bounds get(const Graph* graph);
		//- custom stuff

	protected:
		void init_objects() override;
		
	private:
		std::string fi_db = "./data/d_db_tree_width.csv";
		std::unordered_map<int, std::unordered_map<long long, Bounds > > db_bounds;

		Bounds get(int num_vertices, long long certificate);
		void save(int num_vertices, long long certificate, Bounds);
	};
}

#endif /* TREE_WIDTH_HH */

