import sys
from find_all_graphs import * 

class FindConnectedGraphs:
	def __init__(self, n):
		self.num_vertices = n
		self.num_graphs = 0
		self.num_connected_graphs = 0

	def generate(self):
		for graph in FindAllGraphs(self.num_vertices).generate(False):
			self.num_graphs += 1
			if graph.is_connected():
				self.num_connected_graphs += 1
				yield graph
		self.print_stats()

	def print_stats(self):
		print("#vertices:", self.num_vertices, " #edges:", self.num_edges,
			  " #graphs:", self.num_graphs, " #conn-graphs:", self.num_connected_graphs)

if __name__ == '__main__':
	[graph.print_adj_matrix() for graph in FindConnectedGraphs(int(sys.argv[1])).generate()]
