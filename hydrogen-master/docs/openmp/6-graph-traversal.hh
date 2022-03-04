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

using namespace ga;

void push_stuff(int num_vertices, std::queue<Graph*>& conn_graphs);
void yo_mama(int num_vertices);

