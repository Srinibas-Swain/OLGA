import numpy as np
import functools as ft

class UGraph:
	def __init__(self, n):
		self.num_vertices = n
		self.adj_mat = np.zeros((self.num_vertices, self.num_vertices), dtype=bool)

	def num_edges(self):
		return np.sum(self.adj_mat.ravel())//2
	def generate_graph_id(self):
		#self.print_adj_matrix()
		self.graph_id = 0
		for col in range(1,self.num_vertices):
			for row in range(col):
				self.graph_id *= 2
				self.graph_id += self.adj_mat[row,col]

	def is_connected(self):
		reached = [False for _ in range(self.num_vertices)]
		reached[0] = True
		self.update_reachability(0, reached)
		result = ft.reduce(lambda result, x: x and result, reached, True)
		return result

	def update_reachability(self, node, reached):
		for nbr in range(self.num_vertices):
			if self.is_edge(node, nbr) and not(reached[nbr]):
				reached[nbr] = True
				self.update_reachability(nbr, reached)

	def set_edge(self, node1, node2):
		self.adj_mat[node1, node2] = True
		self.adj_mat[node2, node1] = True

	def reset_edge(self, node1, node2):
		self.adj_mat[node1, node2] = False
		self.adj_mat[node2, node1] = False

	def is_edge(self, node1, node2):
		return self.adj_mat[node1, node2] or self.adj_mat[node2, node1]

	def degree(self, node):
		return ft.reduce(lambda degree_out, x: int(self.is_edge(node,x)) + degree_out, range(self.num_vertices), 0)

	def neighbors(self, node):
		return [node2 for node2 in range(self.num_vertices) if self.is_edge(node, node2)]


	def print_adj_matrix(self):
		for row in range(self.num_vertices):
			for col in range(self.num_vertices):
				if col <= row:
					print('*', end=' ')
				else:
					print(int(self.is_edge(row, col)), end=' ')
			print('')
		print('')

	@staticmethod
	def graph_from_edges(args):
		num_vertices = args['num_vertices']
		graph = UGraph(num_vertices)
		ix_edge = 0
		for col in range(1, num_vertices):
			for row in range(col):
				if args['edges'][ix_edge]:
					graph.set_edge(row, col)
				else:
					graph.reset_edge(row,col)
				ix_edge += 1
		return graph

	@classmethod
	# "n : 1,3,7 : 0,2,4 : 1,3,6 : 0,2,4 : 1,3,5 : 4,6,7 : 2,5,7 : 0,5,6"
	def from_string(cls, n, str_adj_list):
		graph = UGraph(n)
		for ix, str_nbrs in enumerate(str_adj_list.split(' : ')):
			nbrs = [int(n) for n in str_nbrs.split(',')]
			[graph.set_edge(ix, nbr) for nbr in nbrs]
		return graph

	@staticmethod
	def k_n(n):
		graph = UGraph(n)
		for row in range(n):
			for col in range(row+1, n):
				graph.set_edge(row, col)
		graph.generate_graph_id()
		return graph
