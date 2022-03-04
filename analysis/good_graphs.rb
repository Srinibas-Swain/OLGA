

def find_good_graphs(graph g, graph[] good_graphs = [])
	return unless g.good?
	good_graphs << g
	return if g.edges.size == g.vertices-1
	for edge in g.edges
		g_new = g.remove_edge(edge)
		find_good_graphs(g_new, good_graphs)
	end
end

def main(int n)
	find_good_graphs(Graph.complete_graph(n) , [])
end


