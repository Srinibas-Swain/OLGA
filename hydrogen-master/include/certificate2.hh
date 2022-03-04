#ifndef __CERTIFICATE_HH
#define __CERTIFICATE_HH

#include "graph.hh"
#include "blocks.hh"
#include "constants.hh"

namespace ga {
class Certificate {
public:
	Graph *graph;
	Certificate();
	~Certificate();
	void add_graph(Graph *g);
	long generate_certificate(Graph *g);
	long generate_certificate2(Graph *g);
	// todo: make this private
	void split_update(Blocks *B,
					  Blocks *S,
					  Block1 *U,
					  Block1 *T,
					  int& j,
					  int& N);
	Blocks* refine(Blocks* A);
	Blocks* canon1(Blocks *p, Blocks *mu=NULL);
	Blocks* canon2(Blocks *p, Blocks *mu=NULL);
	// private:
	Blocks *L;

private:
	Blocks* initialize_p(int n);
};
}

#endif
