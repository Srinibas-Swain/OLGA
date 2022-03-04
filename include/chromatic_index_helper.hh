#ifndef CHROMATIC_INDEX_HELPER_HH
#define CHROMATIC_INDEX_HELPER_HH

#include "menu_option.hh"
#include "menu_option_helper.hh"

#include <string>
#include <vector>

#include "chromatic_index.hh"
#include "find_connected_components.hh"
#include "graph_pool.hh"
#include "vector_fixed.hh"
#include "vector_int_pool.hh"

namespace ga {
	class ChromaticIndexHelper : public MenuOptionHelper {
	public:
		ChromaticIndexHelper(ChromaticIndex* _ci_mo);
		~ChromaticIndexHelper();

		void init(int n) override;
		void process_graph(const Graph *);
		int find_exact_chromatic_index(const Graph* graph);

	private:
		GraphPool* graph_pool;
		VectorIntPool* vi_pool;
		ChromaticIndex* ci_mo;
		std::string fi_db = ".ci_db.csv";
		CertificateCache2 *cert_cache;
		int i_verbosity;
		std::string str_trash;
		int *hist_S;
		VectorFixed* vertices_t_complement;
		bool ci_debug;
		
		std::pair<std::pair<int, bool>, std::vector<int> > get_degrees(Graph const* graph);
		
		std::pair<int, int> process_removed_node(const Graph* graph_parent,
												 Graph* graph_child,
												 int ix_node,
												 int p_max_degree,
												 std::vector<int> degrees);
		int find_chromatic_index(Graph const* graph);
		int find_max_ci(std::vector<Graph*> conn_components);
			
		bool has_max_degree_nbr(const Graph* graph,
								int node,
								int p_max_degree,
								std::vector<int> degrees);
		bool is_nbr(const Graph* graph, int node1, int node2);

		// skipping the parameter C for now
		void exact_ci_helper(const Graph* graph, int n, int& g, ga::Block* T, Blocks* B, int num_vertices);
		ga::Block* compute_t_complement(const Graph* graph, ga::Block* T, int* map_for_exact_ci);
		Graph* get_induced_graph(const Graph* graph, Block* t_complement);
		Blocks* compute_S(Graph* graph_induced, int num_vertices, int* map_for_exact_ci);
		int least_in_mis(Blocks* S, int n);
		Blocks* compute_S_u(Blocks* S, int u);		
	};
}

#endif /* CHROMATIC_INDEX_HELPER_HH */

