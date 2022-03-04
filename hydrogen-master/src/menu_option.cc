#include "menu_option.hh"

#include <iostream>

#include "block_pool.hh"
#include "partition_pool.hh"
#include "permutation_pool.hh"

using namespace ga;

MenuOption::MenuOption() {
	certificate = new Certificate();
	cert_cache = new CertificateCache2();
	ncr = new Ncr();
}

MenuOption::~MenuOption() {
	// DEBUG3 std::cout << "~Menuoption" << std::endl;
	// forix(get_number_of_threads())
	// delete(mo_helper[ix]);
	delete(ncr);
	delete(cert_cache);
	delete(certificate);
}

void MenuOption::print_args_input_info()
{
	std::cout << "Executing " << title << " ... " << std::endl;
}

void MenuOption::get_args_input(char c_split)
{
	std::string str_input;

	//std::cin.ignore();
	std::getline(std::cin, str_input, '\n');
	//std::cout << "ip: " << str_input; NEWLINE;

	std::istringstream split(str_input);

	for (std::string str_each;
		 std::getline(split, str_each, c_split);
		 input_tokens.push_back(str_each));
}

void MenuOption::init_for_new_n(int in_vertices)
{
	// BlockPool::instance.init_pool(in_vertices, 128);
	// PartitionPool::instance.init_pool(in_vertices, 16);
	// PermutationPool::instance.init_pool(in_vertices, 50);
	//certs_done.clear();
}

void MenuOption::process()
{
	//- should never be called
}

void MenuOption::process_graphs(GraphGenerator::Direction direction)
{
	const Graph* graph_thread;
	long long cert;

	init();
	std::cout << "mo.process: ix_start: " << ix_start << ", ix_end: " << ix_end << std::endl;

	GraphGenerator* graph_gen;
	GraphSeed* graph_seed;
	CertificateCache2 *cert_cache2 = cert_cache;
	int n_connected_graphs;
	
	for(int num_vertices=ix_start; num_vertices <= ix_end; num_vertices++) {
		init_for_new_n(num_vertices); // this should be implemented by mo child

		std::map<long long, bool> certs_done;
		n_connected_graphs = 0;
		graph_gen = new GraphGenerator(num_vertices, ncr, direction);

		omp_set_num_threads(get_number_of_threads());
		auto start = get_time::now();

		//-# pragma omp parallel shared(certs_done, graph_gen, cert_cache) private(graph_thread, graph_seed, cert)
		# pragma omp parallel shared(certs_done, graph_gen, cert_cache2) private(graph_thread, graph_seed, cert)
		{
			#pragma omp for
			for(int ix_graph=0; ix_graph<graph_gen->num_graphs; ix_graph++) {
				#pragma omp critical(gen_graph)
				{
					// graph_gen should return r, ix_comb but that gives away its implementation
					// we can ask graph_gen to return a graph, but this involves too much overhead.
					// that is, amdahl's law kicks in.
					// so have this return a graph-generator-skeleton
					// which has the implementation details which can be used by the mo-helper
					// which runs graph-generation on seperate threads
					graph_seed = graph_gen->next();
				}
				graph_thread = graph_seed->sprout_graph(ncr);

				if(graph_thread->connected()) {
					cert = mo_helpers[omp_get_thread_num()]->generate_certificate(graph_thread);
					#pragma omp critical(save_cert)
					{
						++n_connected_graphs;
						//std::cout << "saving " << graph_thread->graph_id << std::endl;
						cert_cache->save(graph_thread, cert);
					}

					//std::cout << "certs-done[" << cert << "]: " << certs_done[cert] << " :: ";
					if(mo_helpers[omp_get_thread_num()]->lock(certs_done, cert)) {
						process_graph(graph_thread);
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
}


void MenuOption::init_objects()
{
	int n = 1; // if not parallel, there is just one thread
	if(is_parallel())
		n = get_number_of_threads();

	//mo_helpers = (AllMaxInducedSubgraphsHelper**) malloc(n * sizeof(AllMaxInducedSubgraphsHelper*));
	mo_helpers = (MenuOptionHelper**) malloc(n * sizeof(MenuOptionHelper*));
	
	FORix(n) {
		mo_helpers[ix] = new MenuOptionHelper();
	}
}

void MenuOption::print_args()
{
	
}

void MenuOption::print_cert_cache() const
{
	std::cout << "print map" << std::endl;
	cert_cache->print();
}

// the things mentioned here should be done by
// the corresponding menu-option children
void MenuOption::init()
{
	// certificate-generators, helpers
	// mo_helpers = new MenuOptionHelper[get_number_of_threads()];
	Graph::init_upper_triangular(ix_end);
	ncr->init(ix_end * (ix_end-1)/2);
	FORix(get_number_of_threads())
		mo_helpers[ix]->init(ix_end);
	init_pools();
}

void
MenuOption::init_pools()
{
	free(BlockPool::instances);
	free(PartitionPool::instances);
	free(PermutationPool::instances);
		
	BlockPool::instances       = (BlockPool**)       malloc((ix_end+1) * sizeof(BlockPool*));
	PartitionPool::instances   = (PartitionPool**)   malloc((ix_end+1) * sizeof(PartitionPool*));
	PermutationPool::instances = (PermutationPool**) malloc((ix_end+1) * sizeof(PermutationPool*));

	for(int n=1; n<=ix_end; n++) {
		BlockPool::instances[n] = new BlockPool();
		BlockPool::instances[n]->init_pool(n, 128);

		PartitionPool::instances[n] = new PartitionPool();
		PartitionPool::instances[n]->init_pool(n, 16);

		PermutationPool::instances[n] = new PermutationPool();
		PermutationPool::instances[n]->init_pool(n, 50);
	}
}

void MenuOption::process_graph(const ga::Graph *graph)
{
}

void MenuOption::post_processing()
{
	
}

long long MenuOption::compute_graph_id_and_certify(const Graph* graph)
{
	return generate_certificate(graph);
}

long long MenuOption::generate_certificate(const Graph* graph)
{
	if(cert_cache->present(graph))
		return get_certificate(graph);
	else {
		// std:: cout <<
		// 	" num-vertices: " << graph->num_vertices <<
		// 	" connected? " << graph->connected() << 
		// 	" graph-id: " << graph->graph_id << std::endl;
		long long l_certificate = certificate->generate_certificate2(graph); // cert_gen[thread]
		cert_cache->save(graph, l_certificate); // serial
		return l_certificate;
	}
}

std::tuple<long long, int> MenuOption::generate_cert_and_automorph(const Graph* graph)
{
	std::tuple<long long, int> cert_automorphs = certificate->generate_cert_and_automorph(graph);
	return cert_automorphs;
}

long long MenuOption::get_certificate(const Graph* graph) const
{
	return cert_cache->get(graph);
}
