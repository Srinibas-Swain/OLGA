

class Degeneracy:
	def __initialize__(self, graph):
		self.num_vertices = graph.num_vertices
		self.L = set()
		self.degrees = [graph.degree(node) for node in range(self.num_vertices)]
		self.D = [set() for _ in range(self.num_vertices)]
		for node in range(self.num_vertices):
			self.D[self.degrees[node]].add(node)

	def find_degeneracy(self):
		k=0
		ix_ne=0
		for _ in range(self.num_vertices):
			ix_ne = self.index_non_empty_D(ix_ne)

			k = max(k, ix_ne)
			v = self.D[ix_ne].pop()
			self.L = [v] + self.L
			
			for w in graph.neighbors(v):
				if self.L.issubset({w}):
					continue
				
				degree_w = self.degrees[w]
				self.D[degree_w] = self.D[degree_w].difference(w)
				self.degrees[w] = self.degrees[w] - 1
				self.D[degree_w-1].add(w)
		return k

	def index_non_empty_D(self):
		for ix_d in range(self.num_vertices):
			if len(self.D[ix_d]) > 0:
				return ix_d
		raise Exception("D has no non-empty block")
