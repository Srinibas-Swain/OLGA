

def canon1(graph, partition, *mu = nil)
	q = refine(graph, partition)
	l = q.non_unit_index
	result = better
	if mu != nil
		result = q.compare(*mu)
	end
	
	if q.size == n
		# update mu
		if mu == nil
			mu = q.clone()
		elsif result == better
			delete(mu)
			mu = q.clone()
		end
		delete(q)
		return
	end
	
	if result != worse
		# c is the list of candidates considered successively into r[l]
		# d is just a cache of q[l], used to fill r[l+1]
		c,d = q[l].clone, q[l].clone
		r = new block of size == q.size+1
		# copy q(0..(l-1)) to r(0..(l-1))
		(0..(l-1)).each { |j| r[j] = q[j].clone }
		# copy q((l+1)..n) to r((l+2)..n)
		((l+1)..(q.size-1)).each do |j|
			r[j+1] = q[j].clone
		end
		
		while c.size != 0
			u = c.first
			r[l] = u
			r[l+1] = d \ u
			canon1(graph, r, mu)
			c = c \ u
		end
		delete(c,d,r)
	end
	delete(q)
	return mu
end


def certificate(graph)
	p = [{0..n}]
	mu = canon1(p)
	k,c = 0, 0
	for j in ((n-1)..1)
		for i in ((j-1)..0)
			if graph.has_edge(mu[i],mu[j])
				C += 2**k
			end
			k += 1
		end
	end
end

