#include "chromatic_index.hh"
#include "chromatic_index_helper.hh"

#include <iomanip>

#include "csv_iterator.hh"

using namespace ga;

ChromaticIndex::ChromaticIndex() {
	title = "Chromatic Index";
	in_anomalies = 0;
	init_objects();
}

void ChromaticIndex::init_objects()
{
	int n = 1; // if not parallel, there is just one thread
	if(is_parallel())
		n = get_number_of_threads();

	//mo_helpers = (AllMaxInducedSubgraphsHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	mo_helpers = (MenuOptionHelper**) malloc(n * sizeof(ChromaticIndexHelper*));
	
	FORix(n) {
		mo_helpers[ix] = new ChromaticIndexHelper(this);
	}
}

ChromaticIndex::~ChromaticIndex() {
	//std::cout << "~ChromaticIndex"; NEWLINE;
}

void ChromaticIndex::print_args_input_info()
{
	MenuOption::print_args_input_info();
	std::cout << "Enter (start, end) of no. of vertices separated by space: ";
}

void ChromaticIndex::get_args_input(char c_split)
{
	print_args_input_info();

	ix_start=3; ix_end=4;
	ix_start=get_start_index();
	ix_end=get_stop_index();
	if(input_tokens.size() >= 1)
		ix_start=std::stoi(input_tokens[0]);
	if(input_tokens.size() >= 2)
		ix_end=std::stoi(input_tokens[1]);

	print_args();
}

void ChromaticIndex::init()
{
	get_args_input(' ');
	MenuOption::init();
	init_base_cases();
}

void ChromaticIndex::init_for_new_n(int num_vertices)
{
	MenuOption::init_for_new_n(num_vertices);
}

void ChromaticIndex::process_graph(const Graph* graph)
{
	((ChromaticIndexHelper*)mo_helpers[omp_get_thread_num()])->process_graph(graph);
}

void ChromaticIndex::process()
{
	MenuOption::process_graphs(GraphGenerator::Direction::from_kn);
}


void ChromaticIndex::post_processing()
{
	long long cert_max = -1;
	for(auto h2nit : db_ci[ix_end]) {
		cert_max = (h2nit.first > cert_max) ? h2nit.first : cert_max;
	}
	int num_digits_max = num_digits(cert_max);

	std::cout << std::endl <<
		"format: " << std::endl <<
		"smd: single max degree" << std::endl <<
		"vi: vizing's theorem violated?" << std::endl <<
		"#<certificate> [ci_lb, ci_ub]{ci} -max-deg- [smd?] [lb!=ub? vi?]" << std::endl << std::endl;
	for(int n=ix_start; n<=ix_end; n++) {
		std::cout << "n: " << n << std::endl;
		for(auto cert_str : db_ci_out[n]) {
			std::cout << "#" << std::left << setfill(' ') << setw(num_digits_max) << cert_str.first << " ";
			std::cout << cert_str.second << std::endl;
		}
		NEWLINE;
	}
	
	std::cout << "\n#anomalies : " << in_anomalies; NEWLINE; NEWLINE;
	write_db_to_file();
}


// compute certificates for base cases and
// update the db with their chromatic index
void ChromaticIndex::init_base_cases()
{
	long long l_certificate;
	Graph* graph;
	int in_nodes;

	
	// 2-graph
	in_nodes = 2;
	init_for_new_n(in_nodes);
	graph = new Graph(in_nodes);
	graph->set_edge(0,1);
	graph->construct_graph_id();
	l_certificate = compute_graph_id_and_certify(graph);
	db_ci[in_nodes][l_certificate] = 1;
	delete(graph);

	
	in_nodes = 3;
	init_for_new_n(in_nodes);
	// 3-graph -- triangle
	graph = new Graph(in_nodes);
	graph->set_edge(0,1);
	graph->set_edge(1,2);
	graph->set_edge(2,0);
	graph->construct_graph_id();
	l_certificate = compute_graph_id_and_certify(graph);
	db_ci[3][l_certificate] = 3;
	delete(graph);
	
	// 3-graph -- vee
	graph = new Graph(in_nodes);
	graph->set_edge(0,1);
	graph->set_edge(1,2);
	graph->construct_graph_id();
	l_certificate = compute_graph_id_and_certify(graph);
	db_ci[3][l_certificate] = 2;
	delete(graph);
}




// // ci helper
// std::vector<int> ChromaticIndex::get_clean_vip_element(int ix_element)
// {
// 	vector<int> vi_yo = vi_pool->get(ix_element); // vi_pool[thread]
// 	FORix(vi_yo.size())
// 		vi_yo[ix] = 0;
// 	return vi_yo;
// }






// alert: will fail for directed graph


// // bool ChromaticIndex::is_ci_present(int num_vertices, long long certificate)
// // {
// // 	return (db_ci.find(num_vertices) != db_ci.end()) &&
// // 		(db_ci[num_vertices].find(certificate) != db_ci[num_vertices].end());
// // }

void ChromaticIndex::read_db_from_file()
{
	std::cout << "reading db from file " << fi_db << " ... "; NEWLINE;
	// std::ifstream file(fi_db);
    // for(CSVIterator csv_iterator(file); csv_iterator != CSVIterator(); ++csv_iterator) {
	// 	std::cout << "4th Element(" << (*csv_iterator)[3] << ")\n";
	// }
}

// todo
void ChromaticIndex::write_db_to_file()
{
	std::cout << "writing db from file " << fi_db << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db);
	myfile << "1,2" << std::endl;
	for(auto it : db_ci) {
		for(auto it2 : it.second) {
			myfile << it.first << "," << it2.first << "," << it2.second << std::endl;
		}
	}
	myfile.close();
}
 
void ChromaticIndex::print_args()
{
	// print all the arguments for this option here
	std::cout << "start_vertex: " << ix_start;
	std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}

