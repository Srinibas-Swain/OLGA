
from .graph import Graph
from .graph_types import GraphTypes


class DGraph(Graph):
    def __init__(self, num_vertices: int):
        Graph.__init__(self, num_vertices)
        self.root: GraphTypes.Vertex = None
        self.nd = [0 for _ in range(self.num_vertices)]

    def add_edge(self, node1: GraphTypes.Vertex, node2: GraphTypes.Vertex) -> None:
        self.data[node1, node2] = True

    def is_edge(self, node1: GraphTypes.Vertex, node2: GraphTypes.Vertex) -> bool:
        return self.data[node1, node2]

    def post_order(self, node: GraphTypes.Vertex=None, visited: GraphTypes.Bools = None) -> GraphTypes.Vertices:
        if node is None:
            assert self.root is not None, "root is none"
            node = self.root
            visited = [False for _ in range(self.num_vertices)]

        post_order_nodes = []
        visited[node] = True
        for nbr in self.neighbors(node):
            if not visited[nbr]:
                post_order_nodes += self.post_order(nbr, visited)
        return post_order_nodes + [node]

    def pre_order(self, node: GraphTypes.Vertex=None) -> GraphTypes.Vertices:
        if node is None:
            assert self.root is not None, "root is none"
            node = self.root
        pre_order_nodes = [node]
        for nbr in self.neighbors(node):
            pre_order_nodes += self.pre_order(nbr)
        return pre_order_nodes

    def form_num_descendants(self, node=None):
        if node is None:
            node = self.root
        self.nd[node] += 1
        for nbr in self.neighbors(node):
            self.form_num_descendants(nbr)
            self.nd[node] += self.nd[nbr]

    def print_viz(self, engine='neato'):
        g_viz = gv.Digraph(engine=engine)
        for node in range(self.num_vertices):
            for nbr in self.neighbors(node):
                g_viz.edge(str(node), str(nbr))
        return g_viz
