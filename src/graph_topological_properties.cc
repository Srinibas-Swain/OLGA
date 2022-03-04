#include "graph_topological_properties.hh"
#include "graph_topological_properties_helper.hh"

#include "csv_iterator.hh"

//-- custom headers - start
//-- custom headers - end

using namespace ga;

GraphTopologicalProperties::GraphTopologicalProperties() {
	title = "Graph Metric Properties";
	init_objects();
}

void GraphTopologicalProperties::init_objects()
{
	int n = 1; // if not parallel, there is just one thread
	if(is_parallel())
		n = get_number_of_threads();

	//mo_helpers = (AllMaxInducedSubgraphsHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	mo_helpers = (MenuOptionHelper**) malloc(n * sizeof(GraphTopologicalPropertiesHelper*));
	
	FORix(n) {
		mo_helpers[ix] = new GraphTopologicalPropertiesHelper(this);
	}
	
	//- custom object init
}

GraphTopologicalProperties::~GraphTopologicalProperties() {
	//-custom object deletion
}

void GraphTopologicalProperties::print_args_input_info()
{
	MenuOption::print_args_input_info();
	std::cout << "<enter the prompt for this option>";
}

void GraphTopologicalProperties::get_args_input(char c_split)
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

void GraphTopologicalProperties::init()
{
	get_args_input(' ');
	std::cout << "----------------------"; NEWLINE;
	MenuOption::init();
	init_base_cases();
}

void GraphTopologicalProperties::init_for_new_n(int num_vertices)
{
	MenuOption::init_for_new_n(num_vertices);
	FORix(get_number_of_threads())
		mo_helpers[ix]->init_for_new_n(num_vertices);
}

void GraphTopologicalProperties::process()
{
	MenuOption::process_graphs(GraphGenerator::Direction::to_kn);
}


void GraphTopologicalProperties::process_graph(const Graph *graph)
{
	((GraphTopologicalPropertiesHelper*)mo_helpers[omp_get_thread_num()])->process_graph(graph);
}

void GraphTopologicalProperties::post_processing()
{
	long long cert;
	NEWLINE; std::cout << "Graph Metric Properties ... "; NEWLINE;
	for(int num_vertices=ix_start; num_vertices <= ix_end; num_vertices++) {
		std::cout << "n: " << num_vertices << std::endl;
		for(auto it_cert : db_gtp[num_vertices]) {
			cert = it_cert.first;
			std::cout <<
				" cert: " << cert <<
				" tprops::  ";
				for(auto prop : it_cert.second) {
					std::cout << prop.first << ": ";
						if(prop.second == std::numeric_limits<int>::max()) {
							std::cout << "inf";
						} else {
							std::cout << prop.second;
						}
					std::cout << ", ";
				}
			std::cout << std::endl;
		}
		NEWLINE;
	}
	
	write_db_to_file();
}


void GraphTopologicalProperties::init_base_cases()
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
	db_gtp[in_nodes][l_certificate]["circumference"] = std::numeric_limits<int>::max();
	delete(graph);
}

GraphTopologicalProperties::Graph_prop_map GraphTopologicalProperties::get(const Graph* graph)
{
	return get(graph->num_vertices, generate_certificate(graph));
}

GraphTopologicalProperties::Graph_prop_map GraphTopologicalProperties::get(int num_vertices, long long certificate)
{
	return db_gtp[num_vertices][certificate];
}

void GraphTopologicalProperties::save(const Graph *graph, GraphTopologicalProperties::Graph_prop_map _gtp)
{
	save(graph->num_vertices, get_certificate(graph), _gtp);
}

void GraphTopologicalProperties::save(int num_vertices, long long certificate, GraphTopologicalProperties::Graph_prop_map _gtp)
{
	db_gtp[num_vertices][certificate] = _gtp;
}

//todo
void GraphTopologicalProperties::read_db_from_file()
{
	std::cout << "reading db from file " << fi_db << " ... "; NEWLINE;
	std::ifstream file(fi_db);
	for(CSVIterator csv_iterator(file); csv_iterator != CSVIterator(); ++csv_iterator) {
		std::cout << "4th Element(" << (*csv_iterator)[3] << ")\n";
	}
}

void GraphTopologicalProperties::write_db_to_file()
{
	std::cout << "writing db from file " << fi_db << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db);
	myfile << "1,2" << std::endl;
	for(auto it_nodes : db_gtp) {
		for(auto it_cert : it_nodes.second) {
			myfile << it_nodes.first << "," << it_cert.first << ",";
			for(auto prop : it_cert.second)
				myfile << prop.first << "," << prop.second << ",";
			myfile << std::endl;
		}
	}
	myfile.close();
}

void GraphTopologicalProperties::print_args()
{
	// print all the arguments for this option here
	// std::cout << "start_vertex: " << ix_start;
	// std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}


