

class Block:
	def __init__(self, nodes=None):
		if nodes is None:
			self.nodes = set()
		else:
			self.nodes = nodes
		pass

	def __str__(self):
		str_tmp  = "{"
		str_tmp += ",".join([str(x) for x in self.nodes])
		str_tmp += "}"
		return str_tmp

	def add_range(self, node_from, node_to, step_size=1):
		[self.nodes.add(node) for node in range(node_from, node_to, step_size)]

	def __len__(self):
		return len(self.nodes)

	def size(self):
		return len(self.nodes)

	def append(self,node):
		self.add(node)

	def add(self, node):
		self.nodes.add(node)

	def pop(self):
		return self.nodes.pop()
		
	def union_update(self, other_blk):
		self.nodes = self.union(other_blk)
		
	def union(self, other_blk):
		return self.nodes.union(other_blk.nodes)

	def intersection(self, arg):
		if type(arg) is list:
			other_set = set(arg)
		elif type(arg) is set:
			other_set = arg
		else:
			raise Exception("arg has to be set/list, not", type(arg))

		return self.nodes.intersection(other_set)
			
	def issubset(self, other_blk):
		return self.nodes.issubset(other_blk.nodes)

	def __sub__(self, arg):
		return self.difference(arg)

	def difference(self, arg):
		blk = Block()
		if type(arg) is set:
			blk.nodes = self.nodes.difference(arg)
		elif type(arg) is Block:
			blk.nodes = self.nodes.difference(arg.nodes)
		else:
			raise Exception("arg not set/Block but",type(arg))

		return blk

	def difference_update(self, arg):
		if type(arg) is set:
			nodes = self.nodes.difference(arg)
		elif type(arg) is Block:
			nodes = self.nodes.difference(arg.nodes)
		else:
			raise Exception("arg not set/Block but",type(arg))
		self.nodes = nodes
		

	def iterate(self):
		for node in self.nodes:
			yield node

	def empty(self):
		return self.size() == 0

	def nonempty(self):
		return not self.empty()

	def get_list(self):
		return list(self.nodes)

	def clone(self):
		blk = Block()
		for node in self.nodes:
			blk.append(node)
		return blk
	
	
	# has to be like {1,2,3,4}
	@classmethod
	def from_string(cls, str_block):
		items = [int(x) for x in str_block[1:-1].split(",")]
		return cls.from_list(items)

	@classmethod
	def from_list(cls, list_nodes):
		return cls.from_set(set(list_nodes))

	@classmethod
	def from_set(cls, set_nodes):
		blk = Block()
		blk.nodes = set_nodes
		return blk
