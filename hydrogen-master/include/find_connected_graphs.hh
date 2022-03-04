#ifndef FIND_CONNECTED_GRAPHS_H
#define FIND_CONNECTED_GRAPHS_H

#include <functional>
#include <map>
#include <string>
#include <queue>
#include <vector>

#include "graph.hh"
#include "menu_option.hh"
#include "ncr.hh"

namespace ga {
class FindConnectedGraphs {
public:
	typedef std::vector<Graph*> graphs;
	graphs connected_graphs;
	long long n_connected_graphs;
	std::map<long long, bool> present;
	FindConnectedGraphs();
	FindConnectedGraphs(int n, MenuOption*);

	virtual void generate(int n, MenuOption*);
	void generate(Graph* graph, graphs& conn_graphs, MenuOption*);
	
	void generate_for_all(int ix_start, int ix_end, MenuOption*);
	void init_before_big_bang(int ix_start, int ix_end);

	virtual void generate_parallel(int n, const std::function<void(Graph* graph)> &f = NULL);

	void print();
	virtual ~FindConnectedGraphs();

	static void for_all_graphs(int, int, MenuOption*);
	static FindConnectedGraphs* get_fcg();
protected:
	Ncr *ncr;

};
}

#endif /* FIND_CONNECTED_GRAPHS_H */
