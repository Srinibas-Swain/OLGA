#ifndef GRAPH_GENERATOR_HH
#define GRAPH_GENERATOR_HH

#include <omp.h>

#include "graph_seed.hh"
#include "ncr.hh"

namespace ga {
	class GraphGenerator {
	public:
		enum class Direction {from_kn, to_kn};

		GraphGenerator(int _num_vertices, Ncr* ncr, Direction direction=Direction::from_kn);
		~GraphGenerator();

		GraphSeed* next();
		
		GraphSeed* next_seed();
		GraphSeed* prev_seed();
		
		long long get_num_graphs() const;

		long long num_graphs;
	private:
		int num_vertices, num_edges;
		Direction direction;
		long long* num_combinations;
		int r_min, r_current, r_max;
		long long ix_comb;
		GraphSeed **graph_seeds;
		
		void compute_num_combinations(Ncr* ncr);
		void allocate_graph_seeds();
		GraphSeed* form_graph_seed();
	};
}

#endif /* GRAPH_GENERATOR_HH */

