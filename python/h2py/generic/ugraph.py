import numpy as np
import functools as ft
import graphviz as gv


class UGraph:
    def __init__(self, n):
        import numpy as np
        self.num_vertices = n
        self.adj_mat = np.zeros((self.num_vertices, self.num_vertices), dtype=bool)

    def num_edges(self):
        return np.sum(self.adj_mat.ravel()) // 2

    def generate_graph_id(self) -> int:
        # self.print_adj_matrix()
        graph_id = 0
        for col in range(1, self.num_vertices):
            for row in range(col):
                graph_id *= 2
                graph_id += self.adj_mat[row, col]
        return graph_id

    def is_connected(self):
        reached = [False for _ in range(self.num_vertices)]
        reached[0] = True
        self.update_reachability(0, reached)
        result = ft.reduce(lambda result, x: x and result, reached, True)
        return result

    def update_reachability(self, node, reached):
        for nbr in range(self.num_vertices):
            if self.is_edge(node, nbr) and not (reached[nbr]):
                reached[nbr] = True
                self.update_reachability(nbr, reached)

    def set_edge(self, node1: int, node2: int):
        self.adj_mat[node1, node2] = True
        self.adj_mat[node2, node1] = True

    def reset_edge(self, node1, node2):
        self.adj_mat[node1, node2] = False
        self.adj_mat[node2, node1] = False

    def is_edge(self, node1, node2):
        return self.adj_mat[node1, node2] or self.adj_mat[node2, node1]

    def degree(self, node):
        return ft.reduce(lambda degree_out, x: int(self.is_edge(node, x)) + degree_out, range(self.num_vertices), 0)

    def neighbors(self, node):
        for node_other in range(self.num_vertices):
            if not self.is_edge(node, node_other):
                continue
            yield node_other

    def edges(self):
        # column major till before diagonal
        for col in range(self.num_vertices):
            for row in range(col):
                if self.is_edge(col, row):
                    yield (row, col)

    def print_adj_matrix(self):
        print(self)

    def __str__(self):
        str_adj_mat = ''
        for row in range(self.num_vertices):
            for col in range(self.num_vertices):
                if col <= row:
                    str_adj_mat += '* '
                else:
                    str_adj_mat += str(int(self.is_edge(row, col))) + ' '
            str_adj_mat += "\n"
        str_adj_mat += "\n"
        return str_adj_mat

    def graph_viz(self, labels=None, engine='neato'):
        if labels is None:
            labels = [str(v) for v in range(self.num_vertices)]

        g = gv.Graph(engine=engine)
        for row, col in self.edges():
            g.edge(labels[row], labels[col])

        return g

    def __sub__(self, arg):
        if type(arg) is int:
            nodes_rm = [arg]
        else:
            raise Exception("i didnt write this code")

        nodes_rm = set(nodes_rm)
        nodes_present = set(range(self.num_vertices)).difference(nodes_rm)
        return self.induced_subgraph(nodes_present)

    def induced_subgraph(self, nodes):
        graph_induced = UGraph(len(nodes))
        for ix_row, node1 in enumerate(nodes):
            for ix_col, node2 in enumerate(nodes):
                if ix_col <= ix_row:
                    continue
                if self.is_edge(node1, node2):
                    graph_induced.set_edge(ix_row, ix_col)
                else:
                    graph_induced.reset_edge(ix_row, ix_col)  # things we do for safety
        return graph_induced

    def connected_components(self):
        visited = [False for _ in range(self.num_vertices)]
        for node in range(self.num_vertices):
            node_group = []
            self.bfs(node, visited, node_group)
            if len(node_group) > 0:
                yield self.induced_subgraph(node_group)

    def bfs(self, node, visited, node_group):
        if visited[node]:
            return
        visited[node] = True
        node_group.append(node)
        for nbr in self.neighbors(node):
            self.bfs(nbr, visited, node_group)

    def complement(self):
        graph_c = UGraph(self.num_vertices)
        for col in range(self.num_vertices):
            for row in range(col):
                if not self.is_edge(row, col):
                    graph_c.set_edge(row, col)
        return graph_c

    def max_ind_sets(self):
        return self.complement().max_cliques()

    def max_cliques(self):
        all_v = set(list(range(self.num_vertices)))
        cliques = []
        self.bron_kerbosch1(set(), all_v, set(), cliques, False)
        return cliques

    def bron_kerbosch1(self, R, P, X, cliques=list(), print_c=False):
        if (P == set()) and (X == set()):
            if print_c:
                print(R)
            cliques.append(R)
            return
        P_dirty = {v for v in P}
        for v in P:
            nbrs_v = set(self.neighbors(v))
            self.bron_kerbosch1(R.union({v}),
                                P_dirty.intersection(nbrs_v),
                                X.intersection(nbrs_v), cliques, print_c)
            P_dirty.difference_update({v})
            X = X.union({v})

    def compute_degrees(self):
        degrees = [0 for _ in range(self.num_vertices)]
        for row, col in self.edges():
            degrees[row] += 1
            degrees[col] += 1
        return degrees

    def clone(self):
        cloned: UGraph = UGraph(self.num_vertices)
        for row, col in self.edges():
            cloned.set_edge(col, row)
        return cloned

    @classmethod
    # "n : 1,3,7 : 0,2,4 : 1,3,6 : 0,2,4 : 1,3,5 : 4,6,7 : 2,5,7 : 0,5,6"
    def from_string(cls, n, str_adj_list):
        graph = UGraph(n)
        for ix, str_nbrs in enumerate(str_adj_list.split(' : ')):
            nbrs = [int(n) for n in str_nbrs.split(',')]
            [graph.set_edge(ix, nbr) for nbr in nbrs]
        return graph

    @staticmethod
    def k_n(n):
        graph = UGraph(n)
        for row in range(n):
            for col in range(row + 1, n):
                graph.set_edge(row, col)
        graph.graph_id = graph.generate_graph_id()
        return graph

    @staticmethod
    def graph_from_edges(args):
        num_vertices = args['num_vertices']
        graph = UGraph(num_vertices)
        for row, col in args['edges']:
            graph.set_edge(row, col)
        graph.id = graph.generate_graph_id()
        return graph

    @staticmethod
    def graph_from_edges(num_vertices: int, edges):
        graph = UGraph(num_vertices)
        for row, col in edges:
            graph.set_edge(row, col)
        graph.id = graph.generate_graph_id()
        return graph

    @classmethod
    def graph_from_graph_id(cls, num_vertices: int, graph_id: int):
        """
        Construct UGraph
        :param num_vertices: number of vertices of the graph
        :param graph_id: graph_id, of the upper triangular adj matrix in column major order
        :return: an instance of UGraph
        """
        graph: UGraph = UGraph(num_vertices)
        for col in reversed(range(1, num_vertices)):
            for row in reversed(range(col)):
                remainder: int = graph_id % 2
                if remainder == 1:
                    graph.set_edge(col, row)
                graph_id //= 2
        return graph



