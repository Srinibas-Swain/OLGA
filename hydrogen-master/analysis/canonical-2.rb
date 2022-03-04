

def cert2(graph, beta, u)
	n = graph.vertices
	(0..(n-1)).each {|j| u[j] = I}
	beta = I
	p = [{0..n}]
	canon2(graph, beta, u, p)
	k, C = 0, 0
	((n-1)..1).each do |j|
		((j-1)..0).each do |i|
			if ((mu[i],mu[j]) in edges(graph))
				C = x + 2**k # what the fuck is x
			end
			k += 1
		end
	end
	return C
end

def canon2(graph, beta, u, partition)
	q = refine(graph, partition)
	l = q.non_unit_index
	result = better
	if mu != nil # bestexist
		result = q.compare(*mu)
	end

	if q.size == n
		# update mu
		if mu == nil # not bestexist
			mu = q.clone()
		elsif result == better
			delete(mu)
			mu = q.clone()
		elsif result == equal # new shit
			(0..(n-1)).each do |i|
				pi2[q[i]] = mu[i]
				enter2(pi2, beta, u)
			end
		end
		delete(q)
		return mu
	end

	if result != worse
		c, d = q[l].clone, q[l].clone
		r = new block of size == q.size+1
		# copy q(0..(l-1)) to r(0..(l-1))
		(0..(l-1)).each { |j| r[j]   = q[j] }
		# copy q((l+1)..n) to r((l+2)..n)
		((l+1)..(q.size)).each { |j| r[j+1] = q[j] }
		while c.size != 0
			u = c.first
			r[l]   = u
			r[l+1] = d \ u
			mu = canon2(graph, r, mu, u)
			(0..l).each {|j| beta_prime[j] = r[j].first}
			j = l
			for y not in beta_prime[0]..beta_prime[l]
				j += 1
				beta_prime[j] = y
			end
			change_base(mu, u, beta_prime)
			for g in u_l
				c = c \ {g[u]}
		end
	end
end
























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
		(0..(l-1)).each { |j| r[j]   = q[j] }
		# copy q((l+1)..n) to r((l+2)..n)
		((l+1)..n).each { |j| r[j+1] = q[j] }
		
		while c.size != 0
			u = c.first
			r[l] = u
			r[l+1] = d \ u
			mu = canon1(graph, r, mu)
		end
		delete(c,d,r)
	end
	delete(q)
end


