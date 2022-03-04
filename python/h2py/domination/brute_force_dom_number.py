
import itertools

from ..generic.graph_types import GraphTypes
from ..generic.ugraph import UGraph


class BruteForceDomNumber:
    """
    Compute the domination number of a graph like a caveman.
    Domination number is the minimal subset of vertices such that vertices not in the subset are adjacent to
    at least one member in the subset
    """
    def __init__(self, graph: UGraph):
        self.graph: UGraph = graph

    def compute(self) -> GraphTypes.Vertices:
        for k in range(1, self.graph.num_vertices+1):
            dom_set = self.verify_k(k)
            if len(dom_set) != 0:
                return dom_set
        raise Exception("Dom set not found for graph: {}".format(self.graph))

    def verify_k(self, k: int) -> GraphTypes.Vertices:
        """
        Check if the size of the dominating set is k. Iterate through all combinations of k-subsets of the set of
        vertices of the graph. If any of these k-subsets is found to be a dominating set, return that k-subset
        :param k: int \in [1, graph.num_vertices]
        :return: dominating k-subset if k is the domination number. empty list otherwise.
        """
        for subset in itertools.combinations(range(self.graph.num_vertices), k):
            if self.is_dom_set(subset):
                return subset
        empty_set: GraphTypes.Vertices = []
        return empty_set

    def is_dom_set(self, subset: GraphTypes.Vertices) -> bool:
        """
        Check if the given subset is a dominating set. Every non-member should be adjacent to at least one vertex
        in the subset.
        :param subset: subset which needs to be checked for domination (if subset is a dominatrix)
        :return: true if subset is a dominating set. false otherwise.
        """
        other_vertices: GraphTypes.VertexSet = set(range(self.graph.num_vertices)).difference(set(subset))
        for other_vertex in other_vertices:
            dominated = False
            for dom_vertex in subset:
                if self.graph.is_edge(other_vertex, dom_vertex):
                    dominated = True
                    break
            if not dominated:
                return False
        return True


if __name__ == '__main__':
    from generic.unique_certificates import UniqueCertificates
    for graph in UniqueCertificates():
        print("n: {} id: {} dom-set: {}".format(graph.num_vertices, graph.generate_graph_id(),
                                                BruteForceDomNumber(graph).compute()))
