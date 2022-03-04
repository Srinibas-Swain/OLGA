def split_update(&b, &s, graph, &j, &N, t, &u)
	n = graph.num_vertices
	L = {}
	b.each do |u|
		h = size(t.intersect(graph.adj(u)))
		L[h] << u
	end

	m = L.num_of_non_empty_blocks
	return if m <= 1

	(((b.size)-1)..(j+1)).each do |h|
		b[m-1+h] = b[h]
	end
	k = 0
	(0..(n-1)).each do |h|
		next if L[h].empty?
		b[j+k] = L[h]
		s[N+k] = L[h]
		u << L[h]
		k += 1
	end
	j += (m-1)
	N += m
end
