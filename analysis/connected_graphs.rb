

def find_conn_graphs(graph g, present, conn_graphs = [])
	return unless g.connected?
	return if present[g.id]
	
	conn_graphs << g
	present[g.id] = true
	
	return if g.edges.size == g.vertices-1
	for edge in g.edges
		g_new = g.clone
		g_new.remove_edge(edge)
		find_conn_graphs(g_new, conn_graphs)
	end
end

def main(int n)
	present = [false] * 2^(n(n-1)/2)
	find_conn_graphs(K_n, present, [])
end


