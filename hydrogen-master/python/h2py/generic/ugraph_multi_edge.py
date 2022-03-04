import numpy as np

from .ugraph import UGraph


class UGraphMultiEdge(UGraph):
    def __init__(self, n):
        super().__init__(n)
        self.num_vertices = n
        self.adj_mat = np.zeros((self.num_vertices, self.num_vertices), dtype=int)

    def edges(self):
        # column major till before diagonal
        for col in range(self.num_vertices):
            for row in range(col+1):  # +1 to ensure we consider self-loops as well
                if self.is_edge(col, row):
                    yield (col, row)

    def num_edges(self) -> int:
        return sum(1 for _ in self.edges())

    def generate_graph_id(self) -> int:
        graph_id = 0
        for col in range(self.num_vertices):
            for row in range(col):
                graph_id *= 2
                graph_id += (1 if self.adj_mat[row, col] > 0 else 0)
        return graph_id

    def set_edge(self, node1: int, node2: int):
        self.adj_mat[node1, node2] += 1
        if node1 != node2:
            self.adj_mat[node2, node1] += 1

    def remove_edge(self, node1, node2):
        self.adj_mat[node1, node2] -= 1
        if node1 != node2:
            self.adj_mat[node2, node1] -= 1

    def reset_edge(self, node1, node2):
        self.adj_mat[node1, node2] = 0
        self.adj_mat[node2, node1] = 0

    def edge_count(self, node1: int, node2: int):
        return self.adj_mat[node1, node2] if(self.adj_mat[node1, node2] > 0) else self.adj_mat[node2, node1]

    def is_edge(self, node1, node2):
        return (self.adj_mat[node1, node2] > 0) or (self.adj_mat[node2, node1] > 0)

    def induced_subgraph(self, nodes):
        graph_induced = UGraphMultiEdge(len(nodes))
        for ix_row, node1 in enumerate(nodes):
            for ix_col, node2 in enumerate(nodes):
                if ix_col <= ix_row:
                    continue
                if self.is_edge(node1, node2):
                    for _ in range(self.edge_count(node1, node2)):
                        graph_induced.set_edge(ix_row, ix_col)
                else:
                    graph_induced.reset_edge(ix_row, ix_col)  # things we do for safety
        return graph_induced

    def complement(self):
        raise Exception("not implemented")

    def max_ind_sets(self):
        raise Exception("not implemented")

    def max_cliques(self):
        raise Exception("not implemented")

    def bron_kerbosch1(self, R, P, X, cliques=list(), print_c=False):
        raise Exception("not implemented")

    def clone(self):
        cloned: UGraphMultiEdge = UGraphMultiEdge(self.num_vertices)
        for col, row in self.edges():
            for _ in range(self.edge_count(col, row)):
                cloned.set_edge(col, row)
        return cloned

    @classmethod
    def from_ugraph(cls, ugraph: UGraph):
        graph = UGraphMultiEdge(ugraph.num_vertices)
        for row, col in ugraph.edges():
            graph.set_edge(col, row)
        return graph

    def __str__(self):
        str_adj_mat = ''
        for row in range(self.num_vertices):
            for col in range(self.num_vertices):
                if col < row:
                    str_adj_mat += '* '
                else:
                    str_adj_mat += '{} '.format(self.edge_count(row, col) if self.is_edge(row, col) else 0)
            str_adj_mat += "\n"
        str_adj_mat += "\n"
        return str_adj_mat[:-2]
