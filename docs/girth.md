* girth_ = $\infty$
* for vertex in graph.vertices:
  * visited, to_visit = set(), list(vertex)
  * vertex.parent = null
  * v.distance = 0
  * distance_ = 0
  * while to_visit.not_empty?:
    * current, to_visit = to_visit[0], to_visit[1:]
    * visited.update(current)
    * distance_ += 1
    * for nbr in graph.nbrs(current):
      * if nbr == current.parent
        * continue
      * if nbr in visited:
        * girth_ = min(girth_, nbr.distance+current.distance+1)
      * else:
        * nbr.distance = current.distance + 1
        * nbr.parent = current
        * to_visit.append(nbr)
* return girth_
