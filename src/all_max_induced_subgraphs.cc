#include "all_max_induced_subgraphs.hh"

#include <chrono>
#include <iomanip>
#include <omp.h>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <time.h>

#include "find_connected_graphs.hh"
#include "graph.hh"
#include "partition_pool.hh"
#include "permutation_pool.hh"

using namespace ga;

AllMaxInducedSubgraphs::AllMaxInducedSubgraphs() {
	title = "All Max Induced Subgraphs";
	init_objects();
}

AllMaxInducedSubgraphs::~AllMaxInducedSubgraphs() {
	// DEBUG3 std::cout << "~AllMaxInducedSubgraphs" << std::endl;
	clean_objects();
}

void AllMaxInducedSubgraphs::print_args_input_info()
{
	MenuOption::print_args_input_info();
	std::cout << "Enter (start, end) of no. of vertices: ";
}

void AllMaxInducedSubgraphs::get_args_input(char c_split)
{
	print_args_input_info();
	//uncomment this to enable input
	//MenuOption::get_args_input(c_split);

	ix_start=3; ix_end=4;
	ix_start=get_start_index();
	ix_end=get_stop_index();
	if(input_tokens.size() >= 1)
		ix_start=std::stoi(input_tokens[0]);
	if(input_tokens.size() >= 2)
		ix_end=std::stoi(input_tokens[1]);

	print_args();
}

bool AllMaxInducedSubgraphs::is_fisg_present(int num_vertices, long long cert)
{
	return (fisgs_by_n__cert.find(num_vertices) != fisgs_by_n__cert.end()) &&
		(fisgs_by_n__cert[num_vertices].find(cert) != fisgs_by_n__cert[num_vertices].end());
}




void AllMaxInducedSubgraphs::init()
{
	get_args_input(' ');
	MenuOption::init();
	init_base_cases();
}

void AllMaxInducedSubgraphs::init_base_cases()
{
	long long l_certificate;
	Graph* graph;
	int in_nodes;
	
	in_nodes = 2;
	init_for_new_n(in_nodes);
	graph = new Graph(in_nodes);
	graph->set_edge(0,1);
	graph->construct_graph_id();
	l_certificate = compute_graph_id_and_certify(graph);
	std::cout << "cert[amis]: " << l_certificate << std::endl;
	//db_ci[in_nodes][l_certificate] = 1;
	delete(graph);
}

void AllMaxInducedSubgraphs::init_for_new_n(int n)
{
	MenuOption::init_for_new_n(n);
}


void AllMaxInducedSubgraphs::process()
{
	MenuOption::process_graphs(GraphGenerator::Direction::from_kn);
}

using ns = chrono::nanoseconds;
using get_time = chrono::system_clock;

void AllMaxInducedSubgraphs::process_graph(const Graph* graph)
{
	mfcis::V_Fisgs_Count v_fisgs =
		((AllMaxInducedSubgraphsHelper*) mo_helpers[omp_get_thread_num()])->process_graph(graph, this);
	#pragma omp critical(fisg_save)
	{
		fisgs_by_n__cert[graph->num_vertices][cert_cache->get(graph)] = v_fisgs;
	}
}

void AllMaxInducedSubgraphs::post_processing()
{
	NEWLINE; NEWLINE;
	std::cout << "format: [num-vertices,#certificate]"; NEWLINE;

	// find max cert-id, for nice formatting
	long long cert_max = -1;
	for(auto h2nit : fisgs_by_n__cert[ix_end]) {
		cert_max = (h2nit.first > cert_max) ? h2nit.first : cert_max;
	}
	int num_digits_max = num_digits(cert_max);
	
	for(int n=ix_start; n<=ix_end; n++) {
		std::cout << "n: " << n << std::endl;
		for(auto fisgs_by_cert : fisgs_by_n__cert[n]) {
			std::cout << "#" << std::left << setfill(' ') << setw(num_digits_max) << fisgs_by_cert.first << " :: ";
				std::cout << " *" << setw(2) << fisgs_by_cert.second.second << " -> ";
			for(auto fisg : fisgs_by_cert.second.first) {
				std::cout << "[" << fisg.first << ", " << "#" << setw(2) << fisg.second  << "] ";
			}
			std::cout << endl;
		}
		NEWLINE;
	}
}

void AllMaxInducedSubgraphs::init_objects()
{
	int n = 1; // if not parallel, there is just one thread
	if(is_parallel())
		n = get_number_of_threads();

	//mo_helpers = (AllMaxInducedSubgraphsHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	mo_helpers = (MenuOptionHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	
	FORix(n) {
		mo_helpers[ix] = new AllMaxInducedSubgraphsHelper();
	}
}

void AllMaxInducedSubgraphs::clean_objects()
{
	int n = 1;
	if(is_parallel())
		n = get_number_of_threads();
	
	FORix(n) {
		delete(mo_helpers[ix]);
	}

	free(mo_helpers);
}

void AllMaxInducedSubgraphs::print_args()
{
	std::cout << "start_vertex: " << ix_start;
	std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}

