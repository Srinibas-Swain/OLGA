
import numpy as np

from .dfs_tree_generator import DfsTreeGenerator
from .dgraph import DGraph
from .graph_types import GraphTypes
from .ugraph import UGraph


class TarjenBridges:
    """
    Tarjan, R. Endre (1974), "A note on finding the bridges of a graph", Information Processing Letters, 2 (6): 160–161,
     doi:10.1016/0020-0190(74)90003-9, MR 0349483.

    * Find a spanning forest of G
    * Create a rooted forest F from the spanning tree
    * Traverse the forest F in preorder and number the nodes. Parent nodes in the forest now have lower numbers than
      child nodes.
    * For each node v in preorder (denoting each node using its preorder number), do:
        * Compute the number of forest descendants ND(v) for this node,
          by adding one to the sum of its children's descendants.
        * Compute L(v), the lowest preorder label reachable from v by a path for which all but the last edge stays
          within the subtree rooted at v. This is the minimum of the set consisting of the preorder label of v, of the
          values of L(w) at child nodes of v and of the preorder labels of nodes reachable from v by edges that do not
          belong to F.
        * Similarly, compute H(v), the highest preorder label reachable by a path for which all but the last edge stays
          within the subtree rooted at v. This is the maximum of the set consisting of the preorder label of v, of the
          values of H(w) at child nodes of v and of the preorder labels of nodes reachable from v by edges that do not
          belong to F.
        * For each node w with parent node v, if L(w)=w and H(w)<w+ND(w) then the edge from v to w is a bridge.
    """

    def __init__(self, graph: UGraph, root: GraphTypes.Vertex = None):
        self.num_vertices: int = graph.num_vertices
        self.root: GraphTypes.Vertex = root
        if self.root is None:
            self.root = 0
        self.dfs_tree: DGraph = DfsTreeGenerator.compute(graph, self.root)

        # order of visitation of the nodes. first node visited corresponds to 0.
        self.numbers_postorder = self.number_nodes_postorder(self.dfs_tree)

        self.num_descendants = [0 for _ in range(self.num_vertices)]
        self.compute_num_descendants(self.dfs_tree, self.root)

        self.back_edges = self.compute_back_edges(graph, self.dfs_tree)
        self.L = [None for _ in range(self.num_vertices)]
        self.H = [None for _ in range(self.num_vertices)]

    def compute(self) -> GraphTypes.EdgeSet:
        """
        The final algorithm in the paper
        """
        self.compute_l(self.root)
        self.compute_h(self.root)
        bridges: GraphTypes.EdgeSet = set()
        for node in range(self.num_vertices):
            for nbr in self.dfs_tree.neighbors(node):
                h_check = self.H[nbr] <= self.numbers_postorder[nbr]
                l_check = self.L[nbr] > self.numbers_postorder[nbr] - self.num_descendants[nbr]
                if h_check and l_check:
                    edge: GraphTypes.Edge = (node, nbr)
                    if node > nbr:
                        edge = (nbr, node)
                    bridges.add(edge)
        return bridges

    def compute_l(self, node):
        """
        Compute L for node as specified in Lemma (3)
        """
        if self.L[node] is not None:
            return self.L[node]
        terms = []
        # v - ND(V) + 1
        terms += [self.numbers_postorder[node] - self.num_descendants[node] + 1]
        # L(w) | v -> w
        terms += [self.compute_l(nbr) for nbr in self.dfs_tree.neighbors(node)]
        # w | v -- w
        terms += self.back_edge_nbr_numbers(node)

        self.L[node] = min(terms)
        return self.L[node]

    def compute_h(self, node):
        """
        Compute H for node as specified in Lemma (4)
        """
        if self.H[node] is not None:
            return self.H[node]
        terms = []
        # v
        terms += [self.numbers_postorder[node]]
        # H(w) | v -> w
        terms += [self.compute_h(nbr) for nbr in self.dfs_tree.neighbors(node)]
        # w | v -- w
        terms += self.back_edge_nbr_numbers(node)

        self.H[node] = max(terms)
        return self.H[node]

    def back_edge_nbr_numbers(self, node: GraphTypes.Vertex) -> GraphTypes.Vector:
        """
        Given a node, find the vertices which are adjacent to node only via backedges.
        Return the NUMBER values of such vertices.
        """
        nbr_numbers: GraphTypes.Vector = []
        back_edges_node = [(node1, node2) for node1, node2 in self.back_edges if (node1 == node) or (node2 == node)]
        for node1, node2 in back_edges_node:
            if node1 == node:
                nbr_numbers.append(self.numbers_postorder[node2])
            else:
                nbr_numbers.append(self.numbers_postorder[node1])
        return nbr_numbers

    def number_nodes_postorder(self, dfs_tree: DGraph) -> GraphTypes.Vector:
        """
        Number the nodes of the graph using the dfs visitation order
        :param dfs_tree: DFS tree for the given graph
        :return: NUMBER as specified by Mr. Tarjen
        """
        numbers_by_node: GraphTypes.Vector = [-1 for _ in range(self.num_vertices)]
        number_current = self.num_vertices-1
        nodes = [dfs_tree.root]
        while len(nodes) > 0:
            node = nodes.pop()
            numbers_by_node[node] = number_current
            number_current -= 1
            nodes.extend(nbr for nbr in dfs_tree.neighbors(node) if numbers_by_node[nbr] < 0)
        assert number_current == -1
        return numbers_by_node

    def compute_num_descendants(self, dfs_tree: DGraph, node: GraphTypes.Vertex) -> None:
        """
        Compute ND (number of descendants) for each node in the given DFS tree. A node is a ancestor of itself.
        For each node, compute the ND values of the children first. Then add them to the current node's ND.
        """
        self.num_descendants[node] += 1  # node is a ancestor / descendant of itself
        for nbr in dfs_tree.neighbors(node):
            self.compute_num_descendants(dfs_tree, nbr)
            self.num_descendants[node] += self.num_descendants[nbr]

    @classmethod
    def compute_back_edges(cls, graph: UGraph, dfs_tree: DGraph) -> GraphTypes.EdgeSet:
        """
        Compute the back edges of the graph wrt to a given DFS tree
        :return: Set of back edges such that for each edge (node1, node2), node1 < node2
        """
        back_edges: GraphTypes.EdgeSet = set()
        for node in range(graph.num_vertices):
            for nbr in graph.neighbors(node):
                if dfs_tree.is_edge(node, nbr) or dfs_tree.is_edge(nbr, node):
                    continue
                edge: GraphTypes.Edge = (node, nbr)
                if node > nbr:
                    edge = (nbr, node)
                back_edges.add(edge)
        return back_edges


if __name__ == '__main__':
    from generic.unique_certificates import UniqueCertificates

    for graph in UniqueCertificates():
        print("n: {} id: {} bridges: {}".format(graph.num_vertices, graph.generate_graph_id(),
                                                TarjenBridges(graph).compute()))
