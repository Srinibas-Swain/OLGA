import itertools

from tqdm import tqdm

from ..generic.graph_types import GraphTypes
from ..generic.graph_utils import GraphUtils
from ..generic.ugraph import UGraph

Edges = GraphTypes.Edges
EdgeSet = GraphTypes.EdgeSet
EdgeNbrhd = GraphTypes.List[Edges]


class LongestPathBruteForce:
    """
    Find the longest path in the given graph with the might force of a brute.
    """

    @classmethod
    def compute(cls, graph: UGraph) -> GraphTypes.Edges:
        vertices: GraphTypes.Vertices = list(range(graph.num_vertices))
        longest_path_so_far = []
        for permutation in itertools.permutations(vertices):
            longest_path = cls.find_longest_subpath(graph, permutation)
            longest_path_so_far = longest_path if len(longest_path) > len(longest_path_so_far) else longest_path_so_far
        return longest_path_so_far

    @classmethod
    def find_longest_subpath(cls, graph: UGraph, vertex_order: GraphTypes.Vertices) -> GraphTypes.Vertices:
        longest_subpath = []
        current_path = [vertex_order[0]]
        for vertex_current in vertex_order[1:]:
            if graph.is_edge(current_path[-1], vertex_current):
                current_path.append(vertex_current)
            else:  # subpath terminated
                longest_subpath = current_path if len(current_path) > len(longest_subpath) else longest_subpath
                current_path = [vertex_current]
        longest_subpath = current_path if len(current_path) > len(longest_subpath) else longest_subpath
        return longest_subpath


if __name__ == '__main__':
    from generic.unique_certificates import UniqueCertificates

    LongestPathBruteForce.compute(list(UniqueCertificates())[1])
    for graph in UniqueCertificates():
        print("n: {} id: {} longest-path: {}".format(graph.num_vertices, graph.generate_graph_id(),
                                                     LongestPathBruteForce.compute(graph)))
