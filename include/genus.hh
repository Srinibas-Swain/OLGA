#ifndef GENUS_HH
#define GENUS_HH

#include "menu_option.hh"

#include <tuple>

namespace ga {
	typedef typename std::tuple<int,int,int> Triplet;
	
	class Genus : public MenuOption {
	public:
		Genus();
		~Genus();

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

		void save(const Graph* graph, Triplet lb_ub_gamma);
		Triplet get(const Graph* graph);

		//- custom stuff
		// wrapper
		long long ncr_get(int n, int r);

	protected:
		void init_objects() override;
		//- custom protected -- start
		
		//- custom protected -- end
	private:
		std::string fi_db = "./data/d_db_genus.csv";
		std::unordered_map<int, std::unordered_map<long long, Triplet> > db_genus;

		Triplet get(int num_vertices, long long certificate);
		void save(int num_vertices, long long certificate, Triplet);

		
	};
}

#endif /* GENUS_HH */

