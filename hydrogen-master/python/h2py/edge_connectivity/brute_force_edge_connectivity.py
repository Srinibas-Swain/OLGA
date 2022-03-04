
import functools as ft
import itertools

from ..generic.graph_types import GraphTypes
from ..generic.ugraph import UGraph


class BruteForceEdgeConnectivity:
    """
    Compute the edge connectivity of a given graph
    Edge connectivity: minimum number of edges that needs to be removed to make the graph disconnected
    """
    def __init__(self, graph: UGraph):
        self.graph = graph
        self.neighbors = [list(self.graph.neighbors(vertex)) for vertex in range(self.graph.num_vertices)]
        self.edges: GraphTypes.EdgeSet = set()
        for row, col in self.graph.edges():
            self.edges.add((row, col))
            # self.edges.add((col, row))

    def compute(self) -> GraphTypes.Edges:
        for num_edges in range(1, len(self.edges)+1):
            bridges: GraphTypes.Edges = self.check_k_bridge_connectivity(num_edges)
            if bridges is not None:
                return bridges
        # raise Exception("No bridges found")
        empty_edge_list: GraphTypes.Edges = []
        return empty_edge_list

    def check_k_bridge_connectivity(self, k) -> GraphTypes.Edges:
        bridges: GraphTypes.Edges = None
        for possible_bridges in itertools.combinations(self.edges, k):
            possible_bridges: GraphTypes.EdgeSet = set(possible_bridges)
            if self.verify_bridges(possible_bridges):
                bridges = possible_bridges
                break
        return bridges

    def verify_bridges(self, bridges: GraphTypes.EdgeSet) -> bool:
        visited = [False for _ in range(self.graph.num_vertices)]
        vertex_queue: list = [0]  # add 0-th vertex to the queue
        while len(vertex_queue) > 0:
            vertex: GraphTypes.Vertex = vertex_queue.pop()
            visited[vertex] = True
            neighbors: list = [nbr for nbr in self.neighbors[vertex]
                               if (not visited[nbr]) and ((vertex, nbr) not in bridges) and
                               ((nbr, vertex) not in bridges)]
            vertex_queue.extend(neighbors)
        all_visited: bool = ft.reduce(lambda acc, x: acc and x, visited, True)
        return not all_visited


if __name__ == '__main__':
    from generic.unique_certificates import UniqueCertificates
    for graph in UniqueCertificates():
        print("n: {} id: {} dom-set: {}".format(graph.num_vertices, graph.generate_graph_id(),
                                                BruteForceEdgeConnectivity(graph).compute()))

