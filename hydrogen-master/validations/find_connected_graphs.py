import sys
from find_all_graphs import * 

class FindConnectedGraphs:
	def __init__(self, n):
		self.num_vertices = n
		self.num_graphs = 0
		self.num_connected_graphs = 0

	def generate(self, b_print_stats=True):
		for graph in FindAllGraphs(self.num_vertices).generate(False):
			self.num_graphs += 1
			if graph.is_connected():
				self.num_connected_graphs += 1
				yield graph
		b_print_stats and self.print_stats()

	def get_stats(self):
		return "#vertices: {0}, #graphs: {1}, #conn-graphs: {2}".format(self.num_vertices,
																		self.num_graphs,
																		self.num_connected_graphs)

	def print_stats(self):
		print(self.get_stats())

if __name__ == '__main__':
	graph_ids = []
	for graph in FindConnectedGraphs(int(sys.argv[1])).generate():
		graph.generate_graph_id()
		print('graph-id:', graph.graph_id)
		graph_ids.append(graph.graph_id)
	graph_ids.sort()

