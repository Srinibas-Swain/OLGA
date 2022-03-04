#include <iostream>
#include <vector>
#include <Eigen/Dense>
// #include <gsl/gsl_sf_bessel.h>

#include "graph.hh"
#include "certificate.hh"
#include "block.hh"
#include "permutation.hh"
#include "schreiersims.hh"

using namespace std;
using namespace Eigen;

// test generators
int Permutation::count = 0;
int Permutation::id_g = 0;
int main()
{
	Permutation *alpha = new Permutation(8, "(0,1,3,7,6,4)(2,5)");
	alpha->print2("alpha");
	Permutation *beta = new Permutation(8, "(0,1,3,2)(4,5,7,6)");
	beta->print2("beta");

	Schreiersims::Permutations generators;
	generators.push_back(alpha);
	generators.push_back(beta);

	Schreiersims *U = new Schreiersims();
	U->generate(generators);

	delete(U);
	//delete(beta);
	//delete(alpha);

	cout << Permutation::count << endl;
	return 0;
}

int main2()
{
	Permutation* perm1 = new Permutation(4, "(1,2,3)");
	Permutation* perm2 = new Permutation(4, "(0,3,1)");
	Permutation* perm3 = (*perm1)*(*perm2);
	perm1->print("a");
	perm1->print2("a");
	perm2->print("b");
	perm2->print2("b");
	perm3->print("a*b");
	perm3->print2("a*b");
	Permutation* tmp = perm1->inverse();
	tmp->print();
	tmp->print2();
	delete(tmp);
	delete(perm1);
	delete(perm2);
	delete(perm3);
	return 0;
}

// create partition with 2 blocks {0}, {1..7}
Partition* fill_a()
{
cout << "filling partition .. " << endl;
Partition* part = new Partition();
Block1* block;
block = new Block1();
block->add_range(1, 7+1, 1);
part->add_block(block);
delete(block);
	

block = new Block1();
block->add_range(0,0+1,1);
part->add_block(block);
delete(block);
	
part->print("A");
return part;
}

int Block1::count = 0;
int Block1::id_g = 0;


// this is for certificate calls
int main1() {
	Graph *g = new Graph("./graph1.txt");
	cout << g->num_vertices << endl;
	Certificate *cert = new Certificate();
	cert->add_graph(g);
	Partition *A;
	// int j=1;
	// fill partition A
	A = fill_a();
	// cert->split_update(B, j, B->at(0));
	// cert->split_update(B, j, B->at(0));
	// cert->refine(A);

	// todo: uncommenting this will lead to seg fault
	// double free
	// coz L's values are assigned to B
	// FYI: there are two blocks in L which
	// will show up in valgrind
	// delete(L);
	delete(A);
	delete(cert);
	delete(g);

	cout << Block1::count << endl;
	return 0;
}
