#include <iostream>
#include <string>
#include <map>
#include "find_connected_graphs.hh"
#include "graph.hh"
#include "block.hh"
#include "permutation.hh"
#include "certificate.hh"
#include "utils.hh"

using namespace std;

// have to remove this
int Block1::id_g = 0;
int Block1::count = 0;
int Permutation::id_g = 0;
int Permutation::count = 0;


int main(int argv, char** argc)
{
	int ix_start = 3, ix_end = 4;
	if(argv>1) ix_start = atoi(argc[1]);
	if(argv>2) ix_end   = atoi(argc[2]);

	cout << ix_start << " to " << ix_end; NEWLINE;
	
	FindConnectedGraphs *fcg;
	Certificate* certificate = new Certificate();
	string str = "11100011110000";
	long long cert;
	for(int i = ix_start; i <= ix_end; i++) {
		fcg = new FindConnectedGraphs(i);
		cout << "total[" << i << "] : ";
		cout << fcg->connected_graphs.size();
		cout << endl;
		for(Graph* graph : fcg->connected_graphs) {
			cert = certificate->generate_certificate(graph);
		}
		delete(fcg);
	}
	
	//fcg->print();
	// cout << str << endl;
	// str.append("5");
	// str.replace(1,1,"5");
	// cout << str << endl;

	// map<string, bool> present;
	// cout << (present.find("5")==present.end()) << endl;
	// present["5"] = true;
	// cout << (present.find("5")==present.end()) << endl;
	// cout << present["5"] << endl;
	
	// Graph* k_n = Graph::K_n(3);
	// k_n->print();
	// cout << k_n->connected() << endl;
	// delete(k_n);
	delete(certificate);
	return 0;
}
