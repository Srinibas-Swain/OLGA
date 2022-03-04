#ifndef GRAPH_COUNTS_HH
#define GRAPH_COUNTS_HH

#include "menu_option.hh"

#include "certificate.hh"
#include "certificate_cache2.hh"
#include "graph.hh"
#include "graph_pool.hh"
#include "find_connected_graphs.hh"
#include "csv_iterator.hh"

namespace ga {
	class GraphCounts : public MenuOption {
	public:
		GraphCounts();
		~GraphCounts();

		void print_args_input_info();
		void get_args_input(char split_char);
		void init();
		void process(const Graph* graph);
		void post_process();
		void print_args();

		void read_db_from_file();
		bool is_db_present();
		void write_db_to_file();

	private:
		int ix_start,ix_end;
		std::unordered_map<int, std::unordered_map<long long, int> > db_ci;
		Certificate* certificate;
		FindConnectedGraphs* fcg;
		GraphPool* graph_pool;
		CertificateCache2 *cert_cache2;
		std::string fi_db = "./data/gc_db.csv";

		void init_base_cases();
		long long compute_graph_id_and_certify(Graph* g);
		void process_unique_graph(Graph* g);
		long long generate_certificate(Graph const* graph);
	};
}

#endif /* GRAPH_COUNTS_HH */

