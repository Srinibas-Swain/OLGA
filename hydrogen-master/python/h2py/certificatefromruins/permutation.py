
class Permutation:
	def __init__(self, n):
		self.n = n
		self.mappings = [None for _ in range(self.n)]

	def clone(self):
		perm = Permutation.identity(self.n)
		for node_from, node_to in enumerate(self.mappings):
			perm[node_from] = node_to
		return perm

	def __len__(self):
		return self.n

	def __setitem__(self, node_from, node_to):
		self.mappings[node_from] = node_to
		
	def __getitem__(self, ix):
		return self.mappings[ix]

	def __str__(self):
		visited = [False for _ in range(self.n)]
		in_visited = 0
		str_tmp = ''
		while in_visited < self.n:
			nodes_unvisited = [ix for ix,val in enumerate(visited) if not val]
			node = nodes_unvisited[0]
			node_group = []
			while visited[node] == False:
				visited[node] = True
				node_group.append(str(node))
				in_visited += 1
				node = self[node]
			if len(node_group) > 1:
				str_tmp += "(" + ",".join(node_group)  +  ")"
			
		return str_tmp

	def inv(self):
		perm = Permutation(self.n)
		for node_from, node_to in enumerate(self.mappings):
			perm[node_to] = node_from
		return perm

	# self is on the left side of * #
	# perm = [self[other[ix]] for ix in range(n)]
	def __mul__(self, other_perm):
		perm = Permutation(self.n)
		for ix in range(self.n):
			perm[ix] = self[other_perm[ix]]
		return perm

	@classmethod
	def identity(cls, n):
		perm = Permutation(n)
		for ix in range(n):
			perm[ix]=ix
		return perm

	@classmethod
	# form: (0,1,3,7,6,4)(2,5)
	def from_string(cls, n, str_perm):
		perm = cls.identity(n)
		groups = [[int(y) for y in x.split(',')] for x in str_perm[1:-1].split(')(')]
		for group in groups:
			for node_from, node_to in zip(group, group[1:]+[group[0]]):
				perm[node_from] = node_to
		return perm
			
