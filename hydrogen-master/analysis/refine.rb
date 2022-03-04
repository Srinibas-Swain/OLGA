

# returns a refined partition
def refine(graph, a)
	n = graph.vertices
	b = a
	(0..(b.size)).each {|N| s[N] = b[N]}
	N = b.size
	u = {0..(n-1)}
	while N != 0
		N -= 1
		t = s[N]
		if t.subset_of(u)
			u = u \ t
			j = 0
			while j < b.size and b.size < n
				if b.size != 1
					split_update(b, s, graph, j, N, t)
				end
				j += 1
			end
			if b.size == n
				return b
			end
		end
	end
	return b
end


