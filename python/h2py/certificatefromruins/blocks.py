

import functools as ft
from block import *

class Blocks:
	def __init__(self, n=0):
		if n==0:
			self.blocks = []
		else:
			self.blocks = [Block() for _ in range(n)]

	def num_non_empty(self):
		return ft.reduce(lambda n,blk: n+(blk.size()>0), self.blocks, 0)

	def increase_by(self, m):
		[self.append(None) for _ in range(m)]

	def append(self, block):
		self.blocks.append(block)

	def clone(self):
		blks = Blocks()
		[blks.append(blk) for blk in self.blocks]
		return blks

	def pop(self):
		return self.blocks.pop()

	def __getitem__(self, ix):
		return self.blocks[ix]

	def __setitem__(self, ix, block):
		self.blocks[ix] = block

	def set(self, ix, block):
		self.blocks[ix] = block
	
	def size(self):
		return len(self.blocks)

	def reverse(self):
		self.blocks.reverse()

	def __str__(self):
		return "".join([blk.__str__() if blk is not None else "{}" for blk in self.blocks])

	@staticmethod
	# input: {1,2,3,4}{5,6,7}
	def from_string(str_blocks):
		block_strings = str_blocks[1:-1].split("}{")
		block_objs = [Block.from_string("{"+blk_str+"}") for blk_str in block_strings]
		blocks = Blocks()
		blocks.blocks = block_objs
		return blocks

