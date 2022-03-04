#include <iostream>
#include <map>
#include <omp.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <queue>
#include <unistd.h>
#include <vector>

#include "graph.hh"

#define BACKWARD_HAS_BFD 1
#include "backward.hpp"

#include "6-graph-traversal.hh"

using namespace ga;

vector<long> Graph::NUM_UPPER_TRIANGULAR = vector<long>();


int main()
{
	std::vector<int> signals;
	signals.push_back(SIGSEGV);
	signals.push_back(SIGABRT);
	backward::SignalHandling sh(signals);
	std::cout << std::boolalpha << "sh.loaded() == " << sh.loaded() << std::endl;
	
	int num_vertices = 4;
	for(int ix=num_vertices; ix<=num_vertices; ix++)
		yo_mama(num_vertices);
	return 0;
}

