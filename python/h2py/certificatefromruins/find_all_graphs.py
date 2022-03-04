import sys
import graph

class FindAllGraphs:
	def __init__(self, _num_vertices):
		self.num_vertices = _num_vertices
		self.num_edges = self.num_vertices * (self.num_vertices-1) // 2
		self.edges = [False for _ in range(self.num_edges)]
		self.num_graphs = 0

	def generate(self, b_print_stats=True):
		_num_edges_present = 0
		while True:
			params=  {'id':self.num_graphs,
					  'num_vertices': self.num_vertices,
					  'num_edges': _num_edges_present,
					  'edges': self.edges}
			yield graph.UGraph.graph_from_edges(params)

			self.num_graphs += 1
			if _num_edges_present == self.num_edges:
				self.print_stats(b_print_stats)
				break

			# similar to how you would do addition
			# stop when there is no carryforward
			for ix_edge in range(self.num_edges-1, -1, -1):
				if self.edges[ix_edge]:
					self.edges[ix_edge] = False
					_num_edges_present -= 1
				else:
					self.edges[ix_edge] = True
					_num_edges_present += 1
					break		

	def print_stats(self, b_print_stats):
		if b_print_stats:
			print("#vertices:", self.num_vertices, " #edges:", self.num_edges, " #graphs:", self.num_graphs)

if __name__ == '__main__':
	[graph.print_adj_matrix() for graph in FindAllGraphs(int(sys.argv[1])).generate()]
