#include <bitset>
#include <chrono>
#include <execinfo.h>
#include <iostream>
#include <map>
#include <omp.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <vector>

// #include "find_connected_graphs.hh"
// #include "frequent_induced_subgraph.hh"
#include "graph.hh"
#include "block.hh"
#include "blocks.hh"
#include "permutation.hh"
#include "certificate.hh"
#include "utils.hh"
#include "partition_pool.hh"
#include "permutation_pool.hh"

#include "menu_option.hh"
// MENU_OPTION_HEADERS::START
#include "all_max_induced_subgraphs.hh"
#include "unique_certificates.hh"
#include "chromatic_index.hh"
#include "degeneracy.hh"
#include "tree_width.hh"
#include "ind_number.hh"
#include "genus.hh"
#include "graph_basic_properties.hh"
#include "graph_topological_properties.hh"
#include "graph_connectivity.hh"
// MENU_OPTION_HEADERS::END

#define BACKWARD_HAS_BFD 1
#include "backward.hpp"

using namespace std;
using namespace ga;
using ns = chrono::nanoseconds;
// using get_time = chrono::steady_clock;
using get_time = chrono::system_clock;

typedef typename std::vector<int>::size_type size_type;
// have to remove this
int ga::Block::id_g = 0;
int Partition::id_g = 0;

long long Schreiersims::id_g = 0;
vector<int> Schreiersims::stats_size = vector<int>(100, 0);
vector<int> Schreiersims::stats_capacity = vector<int>(100, 0);
size_t Permutation::id_g = 0;
size_t Permutation::count = 0;
size_type Blocks::id_g = 0;
size_type Blocks::count = 0;
vector<long> Graph::NUM_UPPER_TRIANGULAR = vector<long>();

const size_type BlockPool::MIN_CAPACITY = 10;

// BlockPool BlockPool::instance = BlockPool();
// PartitionPool PartitionPool::instance = PartitionPool();
// PermutationPool PermutationPool::instance = PermutationPool();

BlockPool**       BlockPool::instances       = (BlockPool**)       malloc(3 * sizeof(BlockPool*));
PartitionPool**   PartitionPool::instances   = (PartitionPool**)   malloc(3 * sizeof(PartitionPool*));
PermutationPool** PermutationPool::instances = (PermutationPool**) malloc(3 * sizeof(PermutationPool*));

// BlockPool BlockPool::instances = new BlockPool();
// PartitionPool PartitionPool::instance = PartitionPool();
// PermutationPool PermutationPool::instance = PermutationPool();

//#define DEBUG2(x) do { std::cerr << #x << ": " << x << std::endl; } while (0)

int main(int argv, char** argc)
{
	// for handling exceptions and printing stacktraces
	std::vector<int> signals;
	signals.push_back(SIGSEGV);
	signals.push_back(SIGABRT);
	backward::SignalHandling sh(signals);
	std::cout << std::boolalpha << "sh.loaded() == " << sh.loaded() << std::endl;
	
	//newline(5);
	cout << "Welcome to Graph Atlas"; NEWLINE;
	print("-",30);

	vector<MenuOption*> options;
	// MENU_OPTIONS::START
	options.push_back(new AllMaxInducedSubgraphs());
	options.push_back(new UniqueCertificates());
	options.push_back(new ChromaticIndex());
	options.push_back(new Degeneracy());
	options.push_back(new TreeWidth());
	options.push_back(new IndNumber());
	options.push_back(new Genus());
	options.push_back(new GraphBasicProperties());
	options.push_back(new GraphTopologicalProperties());
	options.push_back(new GraphConnectivity());
	// MENU_OPTIONS::END
	
	int ix_menu_item=1;
	for(auto option : options) {
		cout << ix_menu_item << ". ";
		cout << option->title; NEWLINE;
		++ix_menu_item;
	}
	cout << "Enter an option: ";

	std::string str_input;
	
	std::cout << "debug level: "    << debug_level(); NEWLINE;
	std::cout << "is parallel: "    << is_parallel(); NEWLINE;
	std::cout << "num threads: "    << get_number_of_threads(); NEWLINE;
	std::cout << "default option: " << get_option(); NEWLINE;
	std::cout << "start index: "    << get_start_index(); NEWLINE;
	std::cout << "stop index: "     << get_stop_index(); NEWLINE;

	int ix_menu_item_chosen =
		is_option_present() ? get_option() : 1;
	
	std::cout << "chosen one: " << ix_menu_item_chosen; NEWLINE;

	// if some option needs FindAllGraphs, then create a wrapper/swich
	// between find-all-graphs and find-all-connected-graphs
	options[ix_menu_item_chosen-1]->process();
	
	// }
	// catch(const runtime_error& error) {
	// 	cout << error.what() << endl;
	// 	backward::StackTrace st; st.load_here(32);
	// 	backward::Printer p; p.print(st);
	// }
	// catch(...) {
	// 	cout << "" << endl;
	// }

	
	
	for(auto option : options)
		delete(option);
	
	return 0;
}
