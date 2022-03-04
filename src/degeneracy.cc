#include "degeneracy.hh"
#include "degeneracy_helper.hh"


//-- custom headers - start
#include <fstream>
#include <iomanip>

#include "csv_iterator.hh"
#include "partition_pool.hh"
#include "permutation_pool.hh"
//-- custom headers - end

using namespace ga;

Degeneracy::Degeneracy() {
	title = "Degeneracy";
	init_objects();
}

void Degeneracy::init_objects()
{
	int n = 1; // if not parallel, there is just one thread
	if(is_parallel())
		n = get_number_of_threads();

	//mo_helpers = (AllMaxInducedSubgraphsHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	mo_helpers = (MenuOptionHelper**) malloc(n * sizeof(DegeneracyHelper*));
	
	FORix(n) {
		mo_helpers[ix] = new DegeneracyHelper(this);
	}

	i_degeneracy_vals = new int[n];
	b_valid_ss_vals = new bool[n];
}

Degeneracy::~Degeneracy() {
	delete[](b_valid_ss_vals);
	delete[](i_degeneracy_vals);
}

void Degeneracy::print_args_input_info()
{
	MenuOption::print_args_input_info();
	std::cout << "vertex range: ";
}

void Degeneracy::get_args_input(char c_split)
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

void Degeneracy::init()
{
	get_args_input(' ');
	std::cout << "----------------------"; NEWLINE;
	MenuOption::init();
	init_base_cases();
}

void Degeneracy::init_for_new_n(int num_vertices)
{
	MenuOption::init_for_new_n(num_vertices);	
}

void Degeneracy::process()
{
	MenuOption::process_graphs(GraphGenerator::Direction::from_kn);
}

void Degeneracy::process_graph(const Graph *graph)
{
	((DegeneracyHelper*)mo_helpers[omp_get_thread_num()])->process_graph(graph);
	// std::cout <<
	// 	"-- [cert: " << l_certificate_vals[omp_get_thread_num()] <<
	// 	"][k: " << i_degeneracy_vals[omp_get_thread_num()] <<
	// 	"][ss: " << b_valid_ss_vals[omp_get_thread_num()] <<
	// 	"]" << std::endl;
}

void Degeneracy::post_processing()
{
	long long cert_max = -1;
	for(auto it_db : db_degen[ix_end]) {
		cert_max = (it_db.first > cert_max) ? it_db.first : cert_max;
	}
	int num_digits_max = num_digits(cert_max);
	
	long long cert;
	NEWLINE;
	std::cout << "format: #cert {degeneracy}  [k(G \ v) <= k(G) <= k(G \ v) + 1, (bv)] " << std::endl;
	NEWLINE;
	for(int num_vertices=ix_start; num_vertices <= ix_end; num_vertices++) {
		std::cout << "n: " << num_vertices << std::endl;
		for(auto it_cert : db_degen[num_vertices]) {
			cert = it_cert.first;
			std::cout <<
				"#" << std::left << setfill(' ') << setw(num_digits_max) << cert <<
				" {" << it_cert.second << "} " << 
			    (db_ss_condition[num_vertices][cert] ? "  " : "bv") << 
				std::endl;
		}
		NEWLINE;
	}
	
	write_db_to_file();
}

void Degeneracy::init_base_cases()
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
	db_degen[in_nodes][l_certificate] = 1;
	delete(graph);
}


//todo
void Degeneracy::read_db_from_file()
{
	std::cout << "reading db from file " << fi_db << " ... "; NEWLINE;
	std::ifstream file(fi_db);
	for(CSVIterator csv_iterator(file); csv_iterator != CSVIterator(); ++csv_iterator) {
		std::cout << "4th Element(" << (*csv_iterator)[3] << ")\n";
	}
}

void Degeneracy::write_db_to_file()
{
	std::cout << "writing db to file " << fi_db << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db);
	myfile << "1,2" << std::endl;
	for(auto it_nodes : db_degen ) {
		for(auto it_cert : it_nodes.second) {
			myfile << it_nodes.first << "," << it_cert.first << "," << it_cert.second << std::endl;
		}
	}
	myfile.close();
}

void Degeneracy::print_args()
{
	std::cout << "start_vertex: " << ix_start;
	std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}



int Degeneracy::get(const Graph* graph)
{
	return get(graph->num_vertices, generate_certificate(graph));
}

int Degeneracy::get(int num_vertices, long long certificate)
{
	return db_degen[num_vertices][certificate];
}

void Degeneracy::save(const Graph *graph, int degeneracy)
{
	save(graph->num_vertices, get_certificate(graph), degeneracy);
}

void Degeneracy::save(int num_vertices, long long certificate, int i_degeneracy)
{
	db_degen[num_vertices][certificate] = i_degeneracy;
}

bool Degeneracy::absent(int num_vertices, long long certificate)
{
	return !present(num_vertices, certificate);
}

bool Degeneracy::present(int num_vertices, long long certificate)
{
	return (db_degen.find(num_vertices) != db_degen.end()) &&
		(db_degen[num_vertices].find(certificate) != db_degen[num_vertices].end());
}

void Degeneracy::save_ss_condition(const Graph *graph, bool _ss_condition)
{
	db_ss_condition[graph->num_vertices][get_certificate(graph)] = _ss_condition;
}

bool Degeneracy::get_ss_condition(const Graph *graph)
{
	return db_ss_condition[graph->num_vertices][get_certificate(graph)];
}
