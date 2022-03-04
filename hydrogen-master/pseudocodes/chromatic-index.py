

def init_base_cases():
	db = {}
	db[graph_1.certificate] = 0
	db[graph_2.certificate] = 1
	db[graph_3__triangle.certificate] = 3
	db[graph_3__vee.certificate] = 2

def chromatic_index(graph):
	conn_components = find_connected_components(graph)
	cis_components = [db[component.certificate] for component in conn_components]
	return max(cis_components)

init_base_cases()
screwed_up_cases = []
for n = range(2,N+1):
	for graph in connected_graphs(n):
		graph_max_degree = max_degree(graph)
		nodes_max_degree = max_degree_nodes(graph)
		cis_current, cis_current_lb = [], []
		for node in graph.nodes:
			g_minus_node = graph \ node
			max_degree_g_m_n = max_degree(g_minus_node)
			ci_g_m_n = chromatic_index(g_minus_node)
			
			if(graph_max_degree==max_degree_g_m_n):
				cis_current << ci_g_m_n+1
			else:
				for node_max_degree in nodes_max_degree:
					if(node in graph.nbrhd(node_max_degree)):
						cis_current << ci_g_m_n + 2
						break
			cis_current_lb << chromatic_index(g_minus_node)
			
		ci_max = max(cis_current_lb)
		ci_min = min(cis_current)
		
		if(ci_max==ci_min):
			db[graph.certificate] = ci_max
		else:
			db[graph.certificate] = max(ci_max, ci_min)
			screwed_up_cases << graph
