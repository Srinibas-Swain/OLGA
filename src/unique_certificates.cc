#include "unique_certificates.hh"

#include <chrono>
#include <iomanip>
#include <map>
#include <omp.h>
#include <set>
#include <sys/time.h>
#include <time.h>

#include <fstream>

#include "certificate.hh"
#include "find_connected_graphs.hh"
#include "partition_pool.hh"
#include "permutation_pool.hh"

using namespace ga;

UniqueCertificates::UniqueCertificates() {
	title = "Unique Certificates";
	init_objects();
}

UniqueCertificates::~UniqueCertificates() {
	// DEBUG3 std::cout << "~UniqueCertificates" << std::endl;
}

void UniqueCertificates::print_args_input_info()
{
	MenuOption::print_args_input_info();
	std::cout << "Enter (start, end) of no. of vertices separated by space: ";
}

void UniqueCertificates::get_args_input(char c_split)
{
	print_args_input_info();
	//uncomment this to enable input
	//MenuOption::get_args_input(c_split);

	ix_start=get_start_index();
	ix_end=get_stop_index();
	if(input_tokens.size() >= 1)
		ix_start=std::stoi(input_tokens[0]);
	if(input_tokens.size() >= 2)
		ix_end=std::stoi(input_tokens[1]);

	print_args();
}

void UniqueCertificates::init()
{
	get_args_input(' ');
	Graph::init_upper_triangular(ix_end);
	init_pools();
}

void
UniqueCertificates::init_pools()
{
	delete(BlockPool::instances);
	delete(PartitionPool::instances);
	delete(PermutationPool::instances);
		
	BlockPool::instances       = (BlockPool**)       malloc((ix_end+1) * sizeof(BlockPool*));
	PartitionPool::instances   = (PartitionPool**)   malloc((ix_end+1) * sizeof(PartitionPool*));
	PermutationPool::instances = (PermutationPool**) malloc((ix_end+1) * sizeof(PermutationPool*));

	for(int n=ix_start; n<=ix_end; n++) {
		//std::cout << "mo: " << 
		BlockPool::instances[n] = new BlockPool();
		BlockPool::instances[n]->init_pool(n, 128);

		PartitionPool::instances[n] = new PartitionPool();
		PartitionPool::instances[n]->init_pool(n, 16);

		PermutationPool::instances[n] = new PermutationPool();
		PermutationPool::instances[n]->init_pool(n, 50);
	}
}

void UniqueCertificates::init_for_new_n(int n)
{
	MenuOption::init_for_new_n(n);
	//std::cout << "uc.init-for-new-n: " << "after mo.init_for_new_n" << std::endl;	
}


using ns = chrono::nanoseconds;
using get_time = chrono::system_clock;
	
void UniqueCertificates::process()
{
	const Graph* graph_thread;
	long long cert;
	int num_automorphs;

	init();
	std::cout << "mo.process: ix_start: " << ix_start << ", ix_end: " << ix_end << std::endl;
	Graph::init_upper_triangular(ix_end);
	ncr->init(ix_end * (ix_end-1)/2);
	//std::cout << "uc.process: " << "after ncr init" << std::endl;

	GraphGenerator* graph_gen;
	GraphSeed* graph_seed;
	CertificateCache2 *cert_cache2 = cert_cache;
	int n_connected_graphs;
	
	for(int num_vertices=ix_start; num_vertices <= ix_end; num_vertices++) {
		init_for_new_n(num_vertices); // this should be implemented by mo child

		std::map<long long, bool> certs_done;
		n_connected_graphs = 0;
		graph_gen = new GraphGenerator(num_vertices, ncr);
		//std::cout << "uc.process: " << "after graph_gen" << std::endl;

		omp_set_num_threads(get_number_of_threads());
		auto start = get_time::now();
		//-# pragma omp parallel shared(certs_done, graph_gen, cert_cache) private(graph_thread, graph_seed, cert)
		# pragma omp parallel shared(certs_done, graph_gen, cert_cache2) private(graph_thread, graph_seed, cert, num_automorphs)
		{
			#pragma omp for
			for(int ix_graph=0; ix_graph<graph_gen->num_graphs; ix_graph++) {
				#pragma omp critical(gen_graph)
				{
					// graph_gen should return r, ix_comb but that gives away its implementation
					// alas, amdahl's law kicks in. so have this return a seed for generating the graph
					graph_seed = graph_gen->next();
				}
				graph_thread = graph_seed->sprout_graph(ncr);
				
				if(false || graph_thread->connected()) {
					std::tuple<long long, int> pair =
						mo_helpers[omp_get_thread_num()]->generate_cert_and_automorph(graph_thread);
					cert = std::get<0>(pair);
					num_automorphs = std::get<1>(pair);
					
					#pragma omp critical(save_cert)
					{
						//std::cout << "graph-id: " << graph_thread->graph_id << " cert: " << cert << std::endl;
						++n_connected_graphs;
						cert_cache->save(graph_thread, cert);
						freq_counter_by_num_vertices[graph_thread->num_vertices].update(cert);
						automorphs_by_n_cert[graph_thread->num_vertices][cert] = num_automorphs;
					}
				}
			}
		}

		auto end = get_time::now();
		auto diff = end - start;
		cout << "total for " << num_vertices << " nodes: ";
			cout << n_connected_graphs << " graphs ";
		cout << "in " << chrono::duration_cast<ns>(diff).count()/1e9  << " seconds ";
		NEWLINE;
		
		delete(graph_gen);
	}
	post_processing(); // this should be at the
	//BlockPool::instance.clear();
}

void UniqueCertificates::process_graph(const ga::Graph *graph)
{
	
}

void UniqueCertificates::post_processing()
{
	NEWLINE;
	std::cout << "format: #<certificate> x<frequency>"; NEWLINE; NEWLINE;
	int num_digits_max;
	long long cert_max = -1;
	for(auto it_map : freq_counter_by_num_vertices) {
		std::cout << "n: " << it_map.first << std::endl;
		cert_max = -1;
		for(auto it_fc : it_map.second) {
			cert_max = (it_fc.first > cert_max) ? it_fc.first : cert_max;
		}
		num_digits_max = num_digits(cert_max);
		for(auto it_fc : it_map.second) {
			std::cout << "#" << std::left << setfill(' ') << setw(num_digits_max) << it_fc.first << " x" << it_fc.second << std::endl;
		}
		NEWLINE;
	}
	validate_frequencies(freq_counter_by_num_vertices);
	save_freq_counter();
	save_automorphs();
	//BlockPool::instance.clear();

}

void UniqueCertificates::validate_frequencies(std::map<int, FrequencyCounter<long>> freq_counter_by_num_vertices)
{
	int n3k[] = {3, 7};
	int n3v[] = {3, 1};
	
	int n4k[] = {7, 13, 15, 30, 31, 63};
	int n4v[] = {4, 12, 12, 3, 6, 1};
	
	int n5k[] = {15, 29, 31, 58, 59, 62, 63, 126, 127, 185, 187, 191, 207, 220, 221, 223, 254, 255, 495, 511, 1023};
	int n5v[] = {5, 60, 30, 60, 60, 60, 60, 10, 10, 60, 60, 20, 15, 12, 60, 60, 30, 30, 15, 10, 1};
	
	int ix;
	for(auto it_map : freq_counter_by_num_vertices) {
		//std::cout << "[n:" << it_map.first << "] ";
		//it_map.second.print();
		ix = 0;
		if(it_map.first == 3) {
			FORix(2) {
				if(it_map.second.get(n3k[ix]) != n3v[ix]) {
					std::ostringstream oss_err;
					oss_err << "n: " << it_map.first << ", k: " << n3k[ix] << ", ";
					oss_err << "actual: " <<  it_map.second.get(n3k[ix]) << ", expected: " << n3v[ix];
					throw std::runtime_error(oss_err.str());
				}
			}
		} else if(it_map.first == 4) {
			FORix(6) {
				if(it_map.second.get(n4k[ix]) != n4v[ix]) {
					std::ostringstream oss_err;
					oss_err << "n: " << it_map.first << ", k: " << n4k[ix] << ", ";
					oss_err << "actual: " <<  it_map.second.get(n4k[ix]) << ", expected: " << n4v[ix];
					throw std::runtime_error(oss_err.str());
				}
			}
		} else if(it_map.first == 5) {
			FORix(21) {
				if(it_map.second.get(n5k[ix]) != n5v[ix]) {
					std::ostringstream oss_err;
					oss_err << "n: " << it_map.first << ", k: " << n5k[ix] << ", ";
					oss_err << "actual: " <<  it_map.second.get(n5k[ix]) << ", expected: " << n5v[ix];
					throw std::runtime_error(oss_err.str());
				}
			}
		}
	}
}

void UniqueCertificates::save_freq_counter()
{
	std::cout << "writing db from file " << fi_db << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db);

	int num_vertices;
	long long cert;
	for(auto it_map : freq_counter_by_num_vertices) {
		num_vertices = it_map.first;
		for(auto it_fc : it_map.second) {
			cert = it_fc.first;
			myfile << num_vertices << "," << cert << std::endl;
		}
	}
	myfile.close();
}

void UniqueCertificates::save_automorphs()
{
	std::cout << "writing db from file " << fi_db_automorphs << " ... ";
	NEWLINE;
	ofstream myfile;
	myfile.open (fi_db_automorphs);

	int num_vertices;
	long long cert;
	int num_automorphs;
	
	for(auto it_map : automorphs_by_n_cert) {
		num_vertices = it_map.first;
		for(auto cert_morphs : it_map.second) {
			cert = cert_morphs.first;
			num_automorphs = cert_morphs.second;
			myfile << num_vertices << "," << cert << "," << num_automorphs << std::endl;
		}
	}
	myfile.close();
}

void UniqueCertificates::print_args()
{
	std::cout << "start_vertex: " << ix_start;
	std::cout << "  end_vertex: " << ix_end;
	NEWLINE;
}

