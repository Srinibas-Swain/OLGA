#include "all_max_induced_subgraphs_helper.hh"

#include "combination_iterator.hh"

using namespace ga;

AllMaxInducedSubgraphsHelper::AllMaxInducedSubgraphsHelper() {
	graph_pool = new GraphPool();
	
}

AllMaxInducedSubgraphsHelper::~AllMaxInducedSubgraphsHelper() {
	delete(graph_pool);
}


/** algo:
	// for frequency of connected subgraphs
	curr_graph_subgraphs = new map<cert, count>()

	group1 = new group();
	group1.add_range(0, graph->num_vertices);
	combination_generator (group1) -*-* this is an iterator
	-*-* for now, use binary iteration
	-*-* upgrade to graycode way to ease things up

	subgraph = graph->clone();
	// for each subset of nodes
	for(auto nodes: combination_generator) {
		// form the induced subgraph
		// remove nodes from graph, update num-vertices, graph-id
		// create a new class subgraph
		// remove nodes: make the entries corresponding to non existent nodes to -1
		subgraph->remove_nodes(nodes) -*-*
		// redefine generate-graph-id: skip -1's'
		subgraph->construct_graph_id() -*-*

		// if connected, then process the fucker
		// this check makes sense only if we are considering the fics
		// of graphs in the increasing order of graph-size
		// else, else... i donno what to do.
		if (graph_id_in_cache?(subgraph->graph_id)) {
			// present => subgraph is connected

			cert_subgraph = get_from_cache(subgraph->graph_id)

			// just init, nothing more
			if (!cert subgraph present in current graph subgraphs)
				current_graph_subgraphs[cert_subgraph] = 0;

			// increment the count
			curr_graph_subgraphs[cert_subgraph] += 1;
		}

		// copy the entries from graph for entries corresponding to nodes
		subgraph->add_nodes(graph, nodes)
	}
	delete(subgraph)

	iterate through curr graph subgraph map
	and find the subgraph in occurring most and
	output its certificate [find max count in the map amongst in values]
**/
//std::vector<std::pair<int,long long> >
//std::pair<vector<std::pair<int, long long> >, long long>
mfcis::V_Fisgs_Count
AllMaxInducedSubgraphsHelper::process_graph(const Graph* graph, const MenuOption* amis)
{
	//opt: cert_cache has to be global, and mutexed
	//fix: should have num_vertices, graph_id as the key
	//fix: not just graph_id
	// cert_cache->save(graph->graph_id, cert_parent);

	// this variable can be at the instance level, but has to be reset for each "node_size"
	// no need to doing init everytime, just when |num_vertices| change
	// can have a check comparing the sizes and init only if required
	graph_pool->init(graph->num_vertices);
	FrequencyCounter<std::pair<int, long long> > freq_counter;

	//for logging
	long long tmp_cert = 15, graph_id = 236;
	std::ostringstream oss_fis;

	CombinationIterator *ci = new CombinationIterator(graph->num_vertices);
	int in_valid_vertices;
	long long certificate;
	VectorFixed* current_vertices;
	Graph* graph_current;
	long long cert_parent = amis->cert_cache->get(graph);

	while(ci->next()) {
		current_vertices = ci->get_current_vertices();
		
		//opt: can store the size of current vertices in ci, since linked_list.size() is O(n)
		in_valid_vertices = current_vertices->size();
		// too small? too big? get lost.
		// induced subgraph w/ one or less nodes is trivial, so ignore
		// induced subgraph w/ same or more nodes as the original should not be processed
		if((in_valid_vertices <= 1) || (in_valid_vertices >= graph->num_vertices))
			continue;

		if(false && (cert_parent == tmp_cert)) {
			oss_fis << "ix: " << ci->get_current_index() << " :: " << current_vertices->get_string();
		}

		// contruct graph_current
		graph_current = graph_pool->get(in_valid_vertices);
		graph->copy(graph_current, current_vertices);
		
		//opt: mutex this read, can lock this based on graph id
		//opt: no lock needed if START == 3
		// if(!(cert_cache->present(graph_current->graph_id))) {
		// 	if(graph_current->connected()) {
		// 		//std::cout << "[." << graph_current->graph_id; 
		// 		certificate = cert_generator->generate_certificate2(graph_current);
		// 		//std::cout << " " << certificate << "]";
		// 		//opt: mutex this write
		// 		cert_cache->save(graph_current->graph_id, certificate);
		// 	}
		// }

		// certificate = cert_by_n__graph_id->get(graph_current->num_vertices, graph_current->graph_id);
		// if(false && (cert_parent == tmp_cert))
		// 	std::cout << "cert-induced: " << certificate  << " conn? " << graph_current->connected() << " going in \n";
		if(amis->cert_cache->present(graph_current)) {
			certificate = amis->cert_cache->get(graph_current);
			if(false && (cert_parent == tmp_cert))
				oss_fis << "cert-induced: " << certificate  << " conn? " << graph_current->connected() << " going in \n";
			freq_counter.update(std::make_pair(in_valid_vertices, certificate));
		} else {
		}
		if(false && (cert_parent == tmp_cert)) oss_fis << std::endl;
	}


	std::pair<vector<std::pair<int, long long> >, long long> cert_max_all = freq_counter.max();
	if(true || (cert_parent == tmp_cert)) {
		oss_fis << "[" << omp_get_thread_num()  << "]" << cert_parent << " :: "  << freq_counter.get_string();
	}
	//- std::cout << oss_fis.str();

	// std::cout << "{ ";
	// for(long long cert1 : cert_max_all) {
	// 	// DEBUG2 std::cout << cert1 << ".";
	// 	cout << cert_cache->rget(cert1) << " ";
	// }
	// std::cout << "}";
	
	freq_counter.clear();
	graph_pool->clear();
	delete(ci);
	return cert_max_all;
}
