#include "tree_width.hh"
#include "tree_width_helper.hh"

#include "csv_iterator.hh"

//-- custom headers - start
#include <iomanip>
//-- custom headers - end

using namespace ga;

TreeWidth::TreeWidth() {
	title = "Tree Width";
	init_objects();
}

void TreeWidth::init_objects()
{
	int n = 1; // if not parallel, there is just one thread
	if(is_parallel())
		n = get_number_of_threads();

	//mo_helpers = (AllMaxInducedSubgraphsHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	mo_helpers = (MenuOptionHelper**) malloc(n * sizeof(TreeWidthHelper*));
	
	FORix(n) {
		mo_helpers[ix] = new TreeWidthHelper(this);
	}
	
	//- custom object init
}

TreeWidth::~TreeWidth() {
	//-custom object deletion
}

void TreeWidth::print_args_input_info()
{
	MenuOption::print_args_input_info();
	//std::cout << "start,end";
}

void TreeWidth::get_args_input(char c_split)
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

void TreeWidth::init()
{
	get_args_input(' ');
	init_base_cases();
	std::cout << "----------------------"; NEWLINE;
	MenuOption::init();
}

void TreeWidth::init_for_new_n(int num_vertices)
{
	MenuOption::init_for_new_n(num_vertices);	
}

void TreeWidth::process()
{
	MenuOption::process_graphs(GraphGenerator::Direction::from_kn);
}


void TreeWidth::process_graph(const Graph *graph)
{
	((TreeWidthHelper*)mo_helpers[omp_get_thread_num()])->process_graph(graph);
}

void TreeWidth::post_processing()
{
	long long cert_max = -1;
	for(auto it_tw : db_bounds[ix_end]) {
		cert_max = (it_tw.first > cert_max) ? it_tw.first : cert_max;
	}
	int num_digits_max = num_digits(cert_max);

	
	long long cert;
	std::cout << std::endl <<
		"format: " << std::endl <<
		"#<certificate> [lb, ub] {tree-width} [bounds violated (bv)]" << std::endl << std::endl;
	bool b_bounds_difft;
	long long num_anamolies = 0;

	for(int num_vertices=ix_start; num_vertices <= ix_end; num_vertices++) {
		std::cout << "n: " << num_vertices << std::endl;
		for(auto it_cert : db_bounds[num_vertices]) {
			cert = it_cert.first;
			b_bounds_difft = (it_cert.second.first != it_cert.second.second);
			num_anamolies += (b_bounds_difft ? 1 : 0);
			std::cout <<
				"#" << std::left << setfill(' ') << setw(num_digits_max) << cert <<
				" [" << it_cert.second.first << "," << it_cert.second.second << "]" <<
				" {" << it_cert.second.second << "}" <<
				(b_bounds_difft ? " bv" : "   ") <<
				std::endl;
		}
		NEWLINE;
	}
	std::cout << "\n#anamolies: " << num_anamolies << std::endl << std::endl;
	write_db_to_file();
}


void TreeWidth::init_base_cases()
{
	long long l_certificate;
	Graph* graph;
	int in_nodes;

	// 2-graph
	//- in_nodes = 2;
	//- init_for_new_n(in_nodes);
	//- graph = new Graph(in_nodes);
	//- graph->set_edge(0,1);
	//- graph->construct_graph_id();
	//- l_certificate = compute_graph_id_and_certify(graph);
	//- db_degen[in_nodes][l_certificate] = 1;
	//- delete(graph);
}

Bounds TreeWidth::get(const Graph* graph)
{
	return get(graph->num_vertices, generate_certificate(graph));
}

Bounds TreeWidth::get(int num_vertices, long long certificate)
{
	return db_bounds[num_vertices][certificate];
}

void TreeWidth::save(const Graph *graph, Bounds bounds)
{
	save(graph->num_vertices, get_certificate(graph), bounds);
}

void TreeWidth::save(int num_vertices, long long certificate, Bounds bounds)
{
	db_bounds[num_vertices][certificate] = bounds;
}

//todo
void TreeWidth::read_db_from_file()
{
	std::cout << "reading db from file " << fi_db << " ... "; NEWLINE;
	std::ifstream file(fi_db);
	for(CSVIterator csv_iterator(file); csv_iterator != CSVIterator(); ++csv_iterator) {
		std::cout << "4th Element(" << (*csv_iterator)[3] << ")\n";
	}
}

void TreeWidth::write_db_to_file()
{
	std::cout << "writing db from file " << fi_db << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db);
	myfile << "1,2" << std::endl;
	for(auto it_nodes : db_bounds) {
		for(auto it_cert : it_nodes.second) {
			myfile << it_nodes.first << "," << it_cert.first << "," << it_cert.second.first << " to " << it_cert.second.second << std::endl;
		}
	}
	myfile.close();
}

void TreeWidth::print_args()
{
	// print all the arguments for this option here
	// std::cout << "start_vertex: " << ix_start;
	// std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}


