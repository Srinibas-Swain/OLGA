from ..generic.graph_types import GraphTypes

Vector = GraphTypes.Vector
Vertex = GraphTypes.Vertex
VertexSet = GraphTypes.VertexSet
Vertices = GraphTypes.Vertices


class ColorationOrder:
    """Variables are ordered such that x_k is connected to more of the variables x_1, \dots, x_{k-1} than to the x_{k+1}
       , \dots, x_N. Ties are broken by choosing the variable with most edges.

       Reference:
       Chromatic Scheduling and the Chromatic Number Problem, J. Randall Brown,
       Management Science 197219:4-part-1 , 456-463
       See: Basic Algorithm at p458
    """
    @classmethod
    def get(cls, graph, vertices: Vertices = None) -> Vertices:
        if vertices is None:
            vertices: Vertices = cls.get_vertices(graph, vertices)

        degrees: Vector = graph.compute_degrees()
        nbrhd: GraphTypes.Dict[int, GraphTypes.Bag] = cls.get_neighborhood(graph)
        # return cls.compute_backtrack(nbrhd, degrees, [], set(range(graph.num_vertices)))
        return cls.compute(graph, nbrhd, degrees)

    @classmethod
    def compute(cls, graph, nbrhd: GraphTypes.Dict[int, GraphTypes.Bag], degrees: Vector) -> Vertices:
        selected_and_candidates = cls.init_for_compute(graph, degrees)
        selected: Vertices = selected_and_candidates[0]
        candidates: VertexSet = selected_and_candidates[1]

        while len(selected) < graph.num_vertices:
            good_candidates: VertexSet = cls.compute_good_candidates(nbrhd, selected, candidates)
            good_candidate: Vertex = cls.choose_among_good_candidates(candidates, good_candidates, degrees)
            candidates.remove(good_candidate)
            selected.append(good_candidate)

        for vx in range(graph.num_vertices):
            assert vx in selected, "selected: {} does not have the vertex {}".format(selected, vx)
        return selected

    @classmethod
    def choose_among_good_candidates(cls, candidates: VertexSet, good_candidates: VertexSet, degrees: Vector) -> Vertex:
        """if no good condidates are found, the max degree vertex among candidates
           if one is found, then that
           else, max degree vertex among the good_candidates
        """
        if len(good_candidates) == 0:
            return max(candidates, key=lambda vx: degrees[vx])
        elif len(good_candidates) == 1:
            return list(good_candidates)[0]

        # break ties by choosing the variable with most edges
        return max(good_candidates, key=lambda vx: degrees[vx])

    @classmethod
    def init_for_compute(cls, graph, degrees) -> (Vertices, VertexSet):
        max_degree: int = max(degrees)
        max_degree_vertices: Vertices = [vx for vx in range(graph.num_vertices) if degrees[vx] == max_degree]
        selected: Vertices = max_degree_vertices[:1]
        candidates: VertexSet = set(range(graph.num_vertices))
        candidates.remove(max_degree_vertices[0])
        return selected, candidates

    @classmethod
    def compute_backtrack(cls, nbrhd: GraphTypes.NbrhdSet, degrees: GraphTypes.Vector,
                          selected: GraphTypes.Vector, candidates: GraphTypes.Bag) -> GraphTypes.Vector:
        if len(candidates) == 0:  # termination condition
            return selected

        good_candidates: GraphTypes.Bag = cls.compute_good_candidates(nbrhd, selected, candidates)
        if len(good_candidates) == 0:
            return []  # backtrack

        for good_candidate in good_candidates:
            candidates.remove(good_candidate)
            selected_now = cls.compute_backtrack(nbrhd, degrees, selected + [good_candidate], candidates)
            if len(selected_now) != 0:
                return selected_now
            candidates.add(good_candidate)

    @classmethod
    def compute_good_candidates(cls, nbrhd, selected: Vertices, candidates: VertexSet) -> VertexSet:
        """Those which have more edges in selected than candidates"""
        if len(selected) == 0:
            return candidates

        candidates_selected: VertexSet = set()
        for candidate in candidates:
            edges_in_bag: int = len(set(selected).intersection(nbrhd[candidate]))
            edges_in_candidates: int = len(candidates.intersection(nbrhd[candidate]))
            if edges_in_bag > edges_in_candidates:
                candidates_selected.add(candidate)
        return candidates_selected

    @classmethod
    def max_degree_vertices(cls, degrees, vertices) -> GraphTypes.Vector:
        max_degree = max(degrees)
        return [vx for vx in vertices if degrees[vx] == max_degree]

    @classmethod
    def get_vertices(cls, graph, vertices: Vertices = None) -> Vertices:
        if vertices is None:
            all_vertices: Vertices = list(range(graph.num_vertices))
            return all_vertices

        assert max(vertices) < graph.num_vertices, "max of vertices should be less that graph.num_vertices"
        return vertices

    @classmethod
    def get_neighborhood(cls, graph) -> GraphTypes.Dict[int, GraphTypes.Bag]:
        nbrhd = dict()
        for vertex in range(graph.num_vertices):
            nbrhd[vertex] = set(graph.neighbors(vertex))
        return nbrhd
