#include "ind_number.hh"
#include "ind_number_helper.hh"

#include "csv_iterator.hh"

//-- custom headers - start
#include <iomanip>
//-- custom headers - end

using namespace ga;

IndNumber::IndNumber() {
	title = "Ind Number";
	init_objects();
}

void IndNumber::init_objects()
{
	int n = 1; // if not parallel, there is just one thread
	if(is_parallel())
		n = get_number_of_threads();

	//mo_helpers = (AllMaxInducedSubgraphsHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	mo_helpers = (MenuOptionHelper**) malloc(n * sizeof(IndNumberHelper*));
	
	FORix(n) {
		mo_helpers[ix] = new IndNumberHelper(this);
	}
	
	//- custom object init
}

IndNumber::~IndNumber() {
	//-custom object deletion
}

void IndNumber::print_args_input_info()
{
	MenuOption::print_args_input_info();
	// std::cout << "<enter the prompt for this option>";
}

void IndNumber::get_args_input(char c_split)
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

void IndNumber::init()
{
	get_args_input(' ');
	MenuOption::init();
	init_base_cases();
	std::cout << "----------------------"; NEWLINE;
}

void IndNumber::init_for_new_n(int num_vertices)
{
	MenuOption::init_for_new_n(num_vertices);	
}

void IndNumber::process()
{
	MenuOption::process_graphs(GraphGenerator::Direction::from_kn);
}


void IndNumber::process_graph(const Graph *graph)
{
	((IndNumberHelper*)mo_helpers[omp_get_thread_num()])->process_graph(graph);
}

void IndNumber::post_processing()
{
	compute_clique_numbers();

	long long cert_max = -1;
	for(auto it_ind_num : db_ind_num[ix_end]) {
		cert_max = (it_ind_num.first > cert_max) ? it_ind_num.first : cert_max;
	}
	int num_digits_max = num_digits(cert_max);
	
	long long cert;
	std::cout << std::endl <<
		"format: " << std::endl <<
		"#<certificate> independence-number, clique-number, vertex-cover" << std::endl << std::endl;
	
	for(int num_vertices=ix_start; num_vertices <= ix_end; num_vertices++) {
		std::cout << "n: " << num_vertices << std::endl;
		for(auto it_cert : db_ind_num[num_vertices]) {
			cert = it_cert.first;
			std::cout <<
				" #" << std::left << setfill(' ') << setw(num_digits_max) << cert <<
				" " << std::get<0>(it_cert.second) <<
				" " << std::get<1>(it_cert.second) <<
				" " << std::get<2>(it_cert.second) <<
				std::endl;
		}
		NEWLINE;
	}
	
	write_db_to_file();
}

// have to parallize
void IndNumber::compute_clique_numbers()
{
	Graph* graph;
	int cli_num;
	Triplet ind_cli_vc;
	long long cert;
	for(int num_vertices=ix_start; num_vertices <= ix_end; num_vertices++) {
		std::cout << "n: " << num_vertices << std::endl;
		for(auto it_cert : db_ind_num[num_vertices]) {
			cert = it_cert.first;
			ind_cli_vc = it_cert.second;

			cli_num = ((IndNumberHelper*)mo_helpers[0])->find_clique_number(num_vertices, cert);

			ind_cli_vc = std::make_tuple(std::get<0>(ind_cli_vc), cli_num, std::get<2>(ind_cli_vc));
			
			db_ind_num[num_vertices][cert] = ind_cli_vc;
		}
	}
}


void IndNumber::init_base_cases()
{
	long long l_certificate;
	Graph* graph;
	int in_nodes;

	// 2-graph
	in_nodes = 2;
	init_for_new_n(in_nodes);
	graph = Graph::K_n(in_nodes);
	graph->construct_graph_id();
	l_certificate = compute_graph_id_and_certify(graph);
	db_ind_num[in_nodes][l_certificate] = std::make_tuple(1,2,1);
	delete(graph);
}

Triplet IndNumber::get(const Graph* graph)
{
	return get(graph->num_vertices, generate_certificate(graph));
}

Triplet IndNumber::get(int num_vertices, long long certificate)
{
	return db_ind_num[num_vertices][certificate];
}

void IndNumber::save(const Graph *graph, Triplet ind_cli_vc)
{
	save(graph->num_vertices, get_certificate(graph), ind_cli_vc);
}

void IndNumber::save(int num_vertices, long long certificate, Triplet ind_cli_vc)
{
	db_ind_num[num_vertices][certificate] = ind_cli_vc;
}

//todo
void IndNumber::read_db_from_file()
{
	std::cout << "reading db from file " << fi_db << " ... "; NEWLINE;
	std::ifstream file(fi_db);
	for(CSVIterator csv_iterator(file); csv_iterator != CSVIterator(); ++csv_iterator) {
		std::cout << "4th Element(" << (*csv_iterator)[3] << ")\n";
	}
}

void IndNumber::write_db_to_file()
{
	std::cout << "writing db from file " << fi_db << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db);
	myfile << "1,2" << std::endl;
	for(auto it_nodes : db_ind_num) {
		for(auto it_cert : it_nodes.second) {
			myfile << it_nodes.first << "," <<
				std::get<0>(it_cert.second) << "," <<
				std::get<1>(it_cert.second) << "," <<
				std::get<2>(it_cert.second) << "," <<
				std::endl;
		}
	}
	myfile.close();
}

void IndNumber::print_args()
{
	// print all the arguments for this option here
	// std::cout << "start_vertex: " << ix_start;
	// std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}


