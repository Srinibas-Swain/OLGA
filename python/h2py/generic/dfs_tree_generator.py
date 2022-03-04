
from .dgraph import DGraph
from .graph_types import GraphTypes
from .ugraph import UGraph

Bools = GraphTypes.Bools


class DfsTreeGenerator:
    """
    Performs a DFS starting with the given node and returns the resulting DFS tree as a DGraph
    """
    @classmethod
    def compute(cls, graph: UGraph, node: GraphTypes.Vertex = 0):
        dfs_tree = DGraph(graph.num_vertices)
        visited: Bools = [False for _ in range(graph.num_vertices)]
        dfs_tree.root = node
        return cls.form_dfs_tree(graph, node, dfs_tree, visited)

    @classmethod
    def form_dfs_tree(cls, graph: UGraph, node: GraphTypes.Vertex, dfs_tree: DGraph, visited: Bools) -> DGraph:
        visited[node] = True
        for nbr in graph.neighbors(node):
            if not visited[nbr]:
                dfs_tree.add_edge(node, nbr)
                cls.form_dfs_tree(graph, nbr, dfs_tree, visited)
        return dfs_tree
