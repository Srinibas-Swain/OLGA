#ifndef __CERTIFICATE_HH
#define __CERTIFICATE_HH

#include "graph.hh"
#include "blocks.hh"
#include "partition.hh"
#include "schreiersims.hh"
#include "constants.hh"

namespace ga {
class Certificate {
public:
	const Graph *graph;
	Partition *mu2;
	Schreiersims *sch;
	
	Certificate();
	~Certificate();
	void add_graph(const Graph *g);
	// long generate_certificate(Graph *g);
	long generate_certificate2(const Graph *g);
	std::tuple<long long, int> generate_cert_and_automorph(const Graph *g);
	// todo: make this private
	void split_update(Partition *B,
					  Blocks *S,
					  ga::Block *U,
					  ga::Block *T,
					  int& j,
					  int& N);
	Partition* refine(const Partition* A);
	Partition* canon1(Partition *p, Partition *mu=NULL);
	Partition* canon2(const Partition *p);
	// private:
	Partition *L;

private:
	Partition* initialize_p(int n);
	int refine_and_find_l(Partition* p, Partition* q);
	void handle_discrete_q(Partition *q, Partition::PartCompare result);
	void initialize_c_d(ga::Block *c, ga::Block *d, Partition *q, int l);
	Partition* initialize_r(Partition *q, int l);
	void update_r(Partition *r, int l, int u, ga::Block *d);
	Permutation* initialize_beta_prime(Partition *q, int l, Partition *r);
	Partition* process_element_of_c(int u, Partition *r, int l, Partition *q, ga::Block *d);
	Partition* handle_result_not_worse(Partition *q, int l);
	void update_automorphism(Partition *q);
	Partition* create_and_update_L(const Graph* graph, ga::Block* block, ga::Block* T);
	void init_schreiersims(int num_vertices);
	int number_of_vertices();
	void canon2_log_p_q_l(const Partition* p, const Partition* q, int l);

	int num_automorphs;
};
}
#endif
