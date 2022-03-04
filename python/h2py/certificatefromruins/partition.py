
from blocks import *
from permutation import *

class Partition(Blocks):
	def __init__(self, n):
		self.n = n
		super(Partition, self).__init__(0)

	def clone(self):
		partition = Partition(self.n)
		[partition.append(blk) for blk in self.blocks]
		return partition

	def is_discrete(self):
		return self.size()==self.n

	def to_permutation(self, N=None):
		if N is None:
			N = self.n
			
		if N == self.n:
			assert self.is_discrete(), 'not a discrete partition'
			
		perm = Permutation(self.n)
		for ix in range(N):
			assert len(self.blocks[ix])==1, 'not a single element'
			perm[ix] = self.blocks[ix].get_list()[0]
		return perm
