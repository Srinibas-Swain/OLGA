#include <iostream>
#include <vector>
#include <Eigen/Dense>
// #include <gsl/gsl_sf_bessel.h>

#include "graph.hh"
#include "certificate.hh"
#include "block.hh"
#include "permutation.hh"
#include "schreiersims.hh"
#include "utils.hh"
#include "constants.hh"

using namespace std;
using namespace Eigen;

int Block1::count = 0;
int Block1::id_g = 0;

int Permutation::count = 0;
int Permutation::id_g = 0;

// this is for certificate calls
int main() {
	Graph *g = new Graph("./graph1.txt");
	cout << g->num_vertices << endl;
	Certificate *cert = new Certificate();
	newline(2);
	long certificate = cert->generate_certificate(g);
	cout << "certificate: " << certificate; newline();
	newline(2);
	delete(cert);
	delete(g);

	cout << "block count: " << Block1::count << endl;
	return 0;
}
