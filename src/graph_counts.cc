#include "graph_counts.hh"

using namespace ga;

GraphCounts::GraphCounts() {
	title = "Graph Counts";
	certificate = new Certificate();
	fcg = new FindConnectedGraphs();
	graph_pool = new GraphPool();
	cert_cache2 = new CertificateCache2();
}

GraphCounts::~GraphCounts() {
	delete(cert_cache2);
	delete(graph_pool);
	delete(fcg);
	delete(certificate);
}

void GraphCounts::print_args_input_info()
{
	MenuOption::print_args_input_info();
	std::cout << "<enter the prompt for this option>";
}

void GraphCounts::get_args_input(char c_split)
{
	print_args_input_info();
	MenuOption::get_args_input(c_split);

	// do the processing of the input args here using "input_tokens" variable
	// which is a vector of strings
	ix_start=3; ix_end=4;
	if(input_tokens.size() >= 1)
		ix_start=std::stoi(input_tokens[0]);
	if(input_tokens.size() >= 2)
		ix_end=std::stoi(input_tokens[1]);

	print_args();
}

void GraphCounts::init()
{
	get_args_input(' ');
	init_base_cases();
	graph_pool->init(ix_end+1);
}


void GraphCounts::post_process()
{
	write_db_to_file();
}

void GraphCounts::process(const Graph* g)
{

}

void GraphCounts::init_base_cases()
{
	long long l_certificate;
	Graph* graph;
	int in_nodes;

	// 2-graph
	in_nodes = 2;
	graph = new Graph(in_nodes);
	graph->set_edge(0,1);
	l_certificate = compute_graph_id_and_certify(graph);
	db_ci[in_nodes][l_certificate] = 1;
	delete(graph);
}

long long GraphCounts::compute_graph_id_and_certify(Graph* graph)
{
	graph->construct_graph_id();
	return generate_certificate(graph);
}

long long GraphCounts::generate_certificate(Graph const* graph)
{
	if(cert_cache2->present(graph->num_vertices, graph->graph_id))
		return cert_cache2->get(graph->num_vertices, graph->graph_id);
	else {
		long long l_certificate = certificate->generate_certificate2(graph);
		cert_cache2->save(graph->num_vertices, graph->graph_id, l_certificate);
		return l_certificate;
	}
}


//todo
void GraphCounts::read_db_from_file()
{
	std::cout << "reading db from file " << fi_db << " ... "; NEWLINE;
	std::ifstream file(fi_db);
	for(CSVIterator csv_iterator(file); csv_iterator != CSVIterator(); ++csv_iterator) {
		std::cout << "4th Element(" << (*csv_iterator)[3] << ")\n";
	}
}

void GraphCounts::write_db_to_file()
{
	std::cout << "writing db from file " << fi_db << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db);
	myfile << "1,2" << std::endl;
	for(auto it_nodes : db_ci) {
		for(auto it_cert : it_nodes.second) {
			myfile << it_nodes.first << "," << it_cert.first << "," << it_cert.second << std::endl;
		}
	}
	myfile.close();
}

void GraphCounts::print_args()
{
	// print all the arguments for this option here
	// std::cout << "start_vertex: " << ix_start;
	// std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}

