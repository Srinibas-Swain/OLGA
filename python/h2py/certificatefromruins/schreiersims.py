
from permutation import *

class Schreiersims:
	def __init__(self, n, base=None):
		self.size = n
		self.Yous = [[Permutation.identity(n)] for _ in range(n)]
		if base is None:
			self.base = Permutation.identity(n)
		else:
			self.base = base

	def exists_h_for_x(self, i, x):
		for h in self.Yous[i]:
			if h[self.base[i]] == x:
				return h
		return None

	def append(self, ix, u):
		self.Yous[ix].append(u)
		
	def __getitem__(self, ix):
		return self.Yous[ix]

	def __iter__(self):
		for Ui in self.Yous:
			yield Ui

	def __str__(self):
		str_tmp = ''
		for ix, Ui in enumerate(self.Yous):
			str_tmp += 'U'+str(ix)+': '
			for u in Ui:
				str_tmp += u.__str__() + ' '
			str_tmp += "\n"
		return str_tmp

	@classmethod
	def enter2(cls, n, g, sch):
		i = cls.test2(n, g, sch)
		if i == n:
			return

		sch.append(i, g)
		for j in range(i+1):
			for h in sch.Yous[j]:
				f = g * h
				cls.enter2(n, f, sch)
			
	@classmethod
	def test2(cls, n, g, sch):
		for i in range(n):
			x = g[sch.base[i]]
			h = sch.exists_h_for_x(i, x)
			if h is not None:
				pi2 = h.inv()
				pi3 = pi2 * g
				for j in range(n):
					g[j] = pi3[j]
			else:
				return i
		return n

	@classmethod
	def change_base(cls, n, sch, base_new, verbosity=0):
		(verbosity>=1) and print('ss change_base to', base_new)
		sch_new = Schreiersims(n, base_new)
		for Uj in sch:
			for g in Uj:
				cls.enter2(n, g.clone(), sch_new)
		return sch_new

	@classmethod
	def gen(cls, n, generators):
		sch = Schreiersims(n)
		for a in generators:
			cls.enter2(n, a, sch)
		print(sch)

	
if __name__ == '__main__':
	alpha = Permutation.from_string(8, "(0,1,3,7,6,4)(2,5)")
	beta = Permutation.from_string(8, "(0,1,3,2)(4,5,7,6)")
	print('alpha', alpha)
	print('beta', beta)
	Schreiersims.gen(8, [alpha, beta])
