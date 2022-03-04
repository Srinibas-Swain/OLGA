#include "genus.hh"
#include "genus_helper.hh"

#include "csv_iterator.hh"

//-- custom headers - start
#include <iomanip>
//-- custom headers - end

using namespace ga;

Genus::Genus() {
	title = "Genus";
	init_objects();
}

void Genus::init_objects()
{
	int n = 1; // if not parallel, there is just one thread
	if(is_parallel())
		n = get_number_of_threads();

	//mo_helpers = (AllMaxInducedSubgraphsHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	mo_helpers = (MenuOptionHelper**) malloc(n * sizeof(GenusHelper*));
	
	FORix(n) {
		mo_helpers[ix] = new GenusHelper(this);
	}
	
	//- custom object init
}

long long Genus::ncr_get(int n, int r)
{
	return ncr->get(n, r);
}

Genus::~Genus() {
	//-custom object deletion
}

void Genus::print_args_input_info()
{
	MenuOption::print_args_input_info();
	//std::cout << "<enter the prompt for this option>";
}

void Genus::get_args_input(char c_split)
{
	print_args_input_info();
	ix_start=3; ix_end=5;
	ix_start=get_start_index();
	ix_end = (ix_end > 5) ? ix_end : 5;
	
	if(input_tokens.size() >= 1)
		ix_start=std::stoi(input_tokens[0]);
	if(input_tokens.size() >= 2)
		ix_end=std::stoi(input_tokens[1]);

	print_args();
}

void Genus::init()
{
	get_args_input(' ');
	std::cout << "----------------------"; NEWLINE;
	MenuOption::init();
	std::cout << "----------------------"; NEWLINE;
	init_base_cases();
	std::cout << "----------------------"; NEWLINE;
}

void Genus::init_for_new_n(int num_vertices)
{
	MenuOption::init_for_new_n(num_vertices);	
}

void Genus::process()
{
	MenuOption::process_graphs(GraphGenerator::Direction::to_kn);
}


void Genus::process_graph(const Graph *graph)
{
	((GenusHelper*)mo_helpers[omp_get_thread_num()])->process_graph(graph);
}

void Genus::post_processing()
{
	long long cert_max = -1;
	for(auto it_genus : db_genus[ix_end]) {
		cert_max = (it_genus.first > cert_max) ? it_genus.first : cert_max;
	}
	int num_digits_max = num_digits(cert_max);
	
	long long cert;
	std::cout << std::endl <<
		"format: " << std::endl <<
		"#<certificate> [lb, ub] {genus} [bound violated? (bv)]" << std::endl << std::endl;
	bool b_bounds_difft;
	long long bound_lower, bound_upper;
	long long num_anamolies = 0;
	
	for(int num_vertices=ix_start; num_vertices <= ix_end; num_vertices++) {
		std::cout << "n: " << num_vertices << std::endl;
		for(auto it_cert : db_genus[num_vertices]) {
			cert = it_cert.first;
			bound_lower = std::get<1>(it_cert.second);
			bound_upper = std::get<0>(it_cert.second)+1;
			b_bounds_difft = (bound_lower != bound_upper);
			num_anamolies += (b_bounds_difft ? 1 : 0);
			
			std::cout <<
				" #" << std::left << setfill(' ') << setw(num_digits_max) << cert <<
				"[" << bound_lower << "," << bound_upper << "]" <<
				" {" << std::get<2>(it_cert.second) << "}" <<
				(b_bounds_difft ? " bv" : "   ") <<
				std::endl;
		}
		NEWLINE;
	}

	std::cout << "\nnumber of anamolies: " << num_anamolies << std::endl << std::endl;
	
	write_db_to_file();
}


// \gamma(k_2)=0 \gamma(k_3)=0 \gamma(k_4)=0 \gamma(k_5)=1
void Genus::init_base_cases()
{
	long long l_certificate;
	Graph* graph;
	int in_nodes;

	std::cout << "-------------------------" << std::endl;
	for(int in_nodes=2; in_nodes<=5; in_nodes++) {
		init_for_new_n(in_nodes);
		graph = Graph::K_n(in_nodes);
		graph->construct_graph_id();
		l_certificate = compute_graph_id_and_certify(graph);
		save(in_nodes, l_certificate, std::make_tuple(-1,-1,0));
		delete(graph);
	}

	std::cout << "-------------------------" << std::endl;

	// K5
	in_nodes = 5;
	init_for_new_n(in_nodes);
	graph = Graph::K_n(in_nodes);
	graph->construct_graph_id();
	l_certificate = compute_graph_id_and_certify(graph);
	save(in_nodes, l_certificate, std::make_tuple(-1,-1,1));
	delete(graph);
}

Triplet Genus::get(const Graph* graph)
{
	return get(graph->num_vertices, generate_certificate(graph));
}

Triplet Genus::get(int num_vertices, long long certificate)
{
	return db_genus[num_vertices][certificate];
}

void Genus::save(const Graph *graph, Triplet lb_ub_genus)
{
	save(graph->num_vertices, get_certificate(graph), lb_ub_genus);
}

void Genus::save(int num_vertices, long long certificate, Triplet lb_ub_genus)
{
	db_genus[num_vertices][certificate] = lb_ub_genus;
}

//todo
void Genus::read_db_from_file()
{
	std::cout << "reading db from file " << fi_db << " ... "; NEWLINE;
	std::ifstream file(fi_db);
	for(CSVIterator csv_iterator(file); csv_iterator != CSVIterator(); ++csv_iterator) {
		std::cout << "4th Element(" << (*csv_iterator)[3] << ")\n";
	}
}

void Genus::write_db_to_file()
{
	std::cout << "writing db from file " << fi_db << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db);
	myfile << "1,2" << std::endl;
	for(auto it_nodes : db_genus) {
		for(auto it_cert : it_nodes.second) {
			myfile << it_nodes.first << "," << it_cert.first << "," 
			" lb " << std::get<0>(it_cert.second) <<
				" ub " << std::get<1>(it_cert.second) <<
				" gamma " << std::get<2>(it_cert.second) <<
				std::endl;
		}
	}
	myfile.close();
}

void Genus::print_args()
{
	// print all the arguments for this option here
	// std::cout << "start_vertex: " << ix_start;
	// std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}


