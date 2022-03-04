#ifndef CHROMATIC_INDEX_HH
#define CHROMATIC_INDEX_HH

#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>


#include "menu_option.hh"

namespace ga {
class ChromaticIndex : public MenuOption {
public:
	ChromaticIndex();
	~ChromaticIndex();

	void print_args_input_info() override;
	void get_args_input(char split_char) override;
	void init() override;
	void init_for_new_n(int n) override;
	void process() override;
	void process_graph(const Graph* graph) override;
	void post_processing() override;
	void print_args() override;

	void init_base_cases();
	//int find_chromatic_index(Graph const* g);

	void read_db_from_file();
	bool is_db_present();
	void write_db_to_file();
	
	struct CINodeAnomaly {
		std::string str_error;
		CINodeAnomaly(const char* err) {
			str_error = std::string(err);
		}
	};

	int in_anomalies;
	std::unordered_map<int, std::unordered_map<long long, int> > db_ci;
	std::unordered_map<int, std::unordered_map<long long, std::string> > db_ci_out;

protected:
	void init_objects() override;
private:
	//auto compare_yo(Graph* graph1, Graph* graph2);
	std::string fi_db = "data/db_chromatic_index.csv";
	int i_verbosity;
	

};
}

#endif /* CHROMATIC_INDEX_HH */
