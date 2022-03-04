import functools as ft
import itertools
import operator

from ..generic.graph_types import GraphTypes
from ..generic.ugraph import UGraph

Edges = GraphTypes.Edges


class MaximumMatchingBruteForce:
    """
    Find the maximum matching of the given graph, like an amoeba
    """
    @classmethod
    def compute(cls, graph: UGraph) -> Edges:
        edges: Edges = list(graph.edges())
        for k in range(len(edges), 0, -1):
            for edge_subset in itertools.combinations(edges, k):
                if cls.verify_maximum_matching(edge_subset):
                    return edge_subset
        raise Exception("no maximum matching found")

    @classmethod
    def verify_maximum_matching(cls, edges: Edges) -> bool:
        num_unique_vertices = len(set(ft.reduce(operator.concat, edges)))
        return num_unique_vertices == len(edges)*2


if __name__ == '__main__':
    from generic.unique_certificates import UniqueCertificates

    for graph in UniqueCertificates():
        print("n: {} id: {} maximum matching: {}".format(graph.num_vertices, graph.generate_graph_id(),
                                                         MaximumMatchingBruteForce.compute(graph)))
