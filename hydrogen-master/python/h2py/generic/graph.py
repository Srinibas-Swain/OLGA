
import numpy as np

from .graph_types import GraphTypes


class Graph:

    def __init__(self, _num_vertices: int):
        self.data = np.zeros((_num_vertices, _num_vertices), dtype=bool)
        self.num_vertices: int = _num_vertices

    def add_edge(self, node1: GraphTypes.Vertex, node2: GraphTypes.Vertex) -> None:
        self.data[node1, node2] = self.data[node2, node1] = True

    def __str__(self) -> str:
        return self.__repr__()

    def neighbors(self, node):
        for nbr in range(self.num_vertices):
            if self.data[node, nbr]:
                yield nbr